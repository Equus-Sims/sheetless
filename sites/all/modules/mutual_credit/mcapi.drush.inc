<?php
// $Id:
/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function mcapi_drush_help($section) {
  switch ($section) {
    case 'mcapi:mcapi-wipeslate':
      return dt('Undo all transactions according to the currency settings');
    case 'drush:mcapi-delete':
      return dt('Completely remove all references to transactions with given serial number');
    case 'drush:mcapi-purge':
      return dt('Remove all erased and reversed transactions and unfreeze the deletemode');
    case 'drush:mcapi-nodedel':
      return dt('remove the exchange nodes following the d6 upgrade');
  }
}


/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @return
 *   An associative array describing your command(s).
 */
function mcapi_drush_command() {
  $items['mcapi-wipeslate'] = array(
    //'callback' => 'drush_mcapi_wipeslate',
    'description' => dt('Trucate the transactions table and fieldAPI fields. USE WITH CARE.'),
    'drupal dependencies' => array('mcapi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'options' => array(
      'purge' => 'Completely remove'
    ),
  );
  $items['mcapi-delete'] = array(
    //'callback' => 'drush_mcapi_delete',
    'description' => dt('Selectively delete transactions. Pass the serial numbers as individual arguments'),
    'drupal dependencies' => array('mcapi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['mcapi-purge'] = array(
    //'callback' => 'drush_mcapi_purge',
    'description' => dt('Remove all erased and reversed transactions and unfreeze the deletemode'),
    'drupal dependencies' => array('mcapi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['mcapi-nodedel'] = array(
    //'callback' => 'drush_mcapi_nodedel',
    'description' => dt('remove the exchange nodes following the d6 upgrade'),
    'drupal dependencies' => array('mcapi'),
    'bootstrap' => DRUPAL_BOOTSTRAP_DATABASE,
  );
  if (function_exists('devel_generate_drush_command')) {
    $items['generate-transactions'] = array(
      //'callback' => 'drush_mcapi_purge',
      'description' => dt('Generate (10) transactions, random users, types, states, dates, currencies, quantities'),
      'drupal dependencies' => array('mcapi'),
      'arguments' => array(
        'num_transactions' => dt('Number of transactions to create. Defaults to 10.'),
      ),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    );
  }
  return $items;
}


function drush_mcapi_wipeslate() {
  if (variable_get('mcapi_controller', 'mcapi') != 'mcapi') {
    drush_print(dt('Purging only supported with default transaction engine.'));
    return;
  }
  if (module_exists('mcapi_signatures')) {
    db_truncate("mcapi_signatures")->execute();
  }
  //how to do "are you sure?" messages in drush!
  //this should be protected with the huser 1 password
  drush_print(dt("@purged transactions purged", array(
    '@purged' => db_query("SELECT count(serial) FROM {mcapi_transactions}")->fetchField(),
  )));

  foreach (field_info_instances('transaction','transaction') as $instance) {
    db_delete('field_data_'.$instance['field_name'])->condition('entity_type', 'transaction')->execute();
    db_delete('field_revision_'.$instance['field_name'])->condition('entity_type', 'transaction')->execute();
  }
  db_truncate('mcapi_transactions')->execute();
  field_cache_clear();
  variable_set('change_undo_mode', TRUE);
}

function drush_mcapi_delete() {
  hard_delete(func_get_args());
  drush_print(dt('Deleted'));
}

function drush_mcapi_purge() {
  $serials = db_select('mcapi_transactions', 't')
    ->fields('t', array('serial'))
    ->condition(db_or()
      ->condition('state', TRANSACTION_STATE_UNDONE)
    )
  ->execute()->fetchCol();
  hard_delete($serials);
  variable_set('change_undo_mode', TRUE);
}

function drush_mcapi_nodedel() {
  $nodes = db_query("SELECT nid FROM {node} WHERE type = 'exchange'")->fetchCol();
  $nids = implode(', ', $nodes);
  db_query("DELETE FROM {node} WHERE nid in ($nids)");
  db_query("DELETE FROM {node_revision} WHERE nid in ($nids)");
  db_query("DELETE FROM {node_access} WHERE nid in ($nids)");
  db_query("DELETE FROM {node_comment_statistics} WHERE nid in ($nids)");
  drush_print('Deleted all traces of exchange nodes from Drupal 6.');
}

function hard_delete($serials) {
  foreach ($serials as $serial) {
    $transaction = transaction_load($serial);
    field_attach_delete('transaction', $transaction);
    module_invoke_all('entity_delete', $transaction, 'transaction');
  }
  entity_get_controller('transaction')->delete($serials, TRUE);
}


function drush_devel_generate_transactions($num_transactions = 10) {
  //get all the possible variables
  $transact_uids = db_query(
    "SELECT uid from {users_roles} WHERE rid IN (:roles)",
    array(':roles' => array_keys(user_roles(FALSE, 'transact')))
  )->fetchCol();
  $transact_uids = array_flip($transact_uids);
  unset($transact_uids[0], $transact_uids[1], $transact_uids[variable_get('intertrading_uid', 0)]);
  $transact_uids = array_flip($transact_uids);

  drush_print('Available users are: '.implode($transact_uids));
  $states = array_keys(mcapi_get_states());
  $types = mcapi_get_types();
  $currencies = currencies_load();
  unset($types['remote'], $types['mass'], $types['dependent']);

  for ($i = 0; $i < $num_transactions; $i++) {
    $uids = $transact_uids;//make sure we don't pick the same uid for payer and payee
    shuffle($uids);
    $props = array(
      'payer' => array_pop($uids),
      'payee' => array_pop($uids),
      'type' => array_rand($types),
      'state' => array_rand($states),
      'quantity' => rand(1, 10),
      'currcode' => array_rand($currencies),
      'created' => rand(strtotime('-1 month'), REQUEST_TIME),
      'description' => 'randomly generated transaction'
    );
    $cluster = array(entity_create('transaction', $props));
    $serial = transaction_cluster_write($cluster, TRUE);
    $transaction = transaction_load($serial);
    $renderable = transaction_view($transaction, variable_get('mcapi_sentence', mcapi_sentence_default()));
    drush_print(strip_tags(drupal_render()));
  }
}
