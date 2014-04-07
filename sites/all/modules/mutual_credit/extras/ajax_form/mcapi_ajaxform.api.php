<?php

/*
 * Example code
 * the ajaxform provided needs to be prepopulated with as full a transaction as possible.
 */
  $node = node_load(8);
  //you can't do transactions with yourself
  if ($node->uid == $GLOBALS['user']->uid) return '';
  //the only way to set the default direction is by setting the payer or payee
  if ($node->want) {
    $props['payer'] = $GLOBALS['user']->uid;
    $props['payee'] = $node->uid;
  }
  else {
    $props['payer'] = $node->uid;
    $props['payee'] = $GLOBALS['user']->uid;
  }
  //any fields which exist both on the passed entity bundle and on the transaction entity
  //will be copied over to prepopulate the transaction entity.
  foreach (field_read_instances(array('bundle' => 'proposition')) as $id => $instance) {
    $props[$instance['field_name']] = field_get_items('node', $node, $instance['field_name']);
  }
  //or maybe the node body is a different field to the designated 'transaction description field'
  if ($fieldname = variable_get('transaction_description_field')) {
    $items = field_get_items('node', $node, 'body');
    //$props[$fieldname][LANGUAGE_NONE] = $items;
    $props['description'] = $items[0]['value'];//these two produce the same
  }
  //don't forget to declare your transaction type in hook_mcapi_info_types
  //or use one of the existing types
  //otherwise there is a 'default' type.
  $props['type']  = 'my_transaction_type';
  //
  //don't forget to declare your transaction states in hook_mcapi_info_states
  //constants are a good idea for those
  //default transaction state is 1, or TRANSACTION_STATE_FINISHED
  $props['state']  = TRANSACTION_STATE_MY

  $transaction = entity_create('transaction', $props);

/*
 * 2 arguments required
 * $transaction, which might be created as above
 * $confirm (bool) gives you an are you sure page
 */
  $transaction = entity_create('transaction', $props);
  return drupal_render(drupal_get_form('mcapi_ajaxform', $transaction, TRUE));



/*
 * implements hook_form_mcapi_ajaxform_alter().
 * hide fields
 * preset fields etc...
 */
function hook_form_mcapi_ajaxform_alter(&$form, &$form_state) {
  $form['#submit'] = 'my_submit_handler';
}
