<?php
// $Id:

/*
 * this is the entityAPI approach
 * normally we would implement hook_views_data
 */
if (class_exists('EntityDefaultViewsController')) {
class transactionDefaultViewsController extends EntityDefaultViewsController {

  /**
   * Defines the result for hook_views_data().
   */
  public function views_data() {
    $data = array();
    $data['mcapi_transactions']['table'] = array(
      'group'  => t('Transaction'),
      'base' => array(
//        'database' => variable_get('mcapi_db_read', 'default'),
        'field' => 'xid',
        'title' => t('Transactions'),
        'help' => t('Records of transactions between users'),
        'access query tag' => 'transaction_view_access',
        'defaults' => array(
          'field' => 'serial',
        ),
      ),
      'entity type' => 'transaction',//so the entity module can expose this table
    );
    $data['mcapi_transactions']['serial'] = array(
      'title' => t('Serial'), // The item it appears as on the UI,
      'field' => array(
        'help' => t('The serial number of the transaction and dependents'),
        'handler' => 'views_handler_field_transaction',
        'additional fields' => array('serial'),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
    $data['mcapi_transactions']['payer'] = array(
      'title' => t('Payer'),
      'help' => t('The user who gave currency in the transaction.'),
      'field' => array(
        'handler' => 'views_handler_field_user_name',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'label' => t('Payer'),
      ),
    );
    $data['mcapi_transactions']['payee'] = array(
      'title' => t('Payee'),
      'help' => t('The user who gave currency in the transaction.'),
      'field' => array(
        'handler' => 'views_handler_field_user_name',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'label' => t('Payee'),
      ),
    );
    $data['mcapi_transactions']['state'] = array(
      'title' => t('State'),
      'help' => t("Transactions move between states, until they are 'finished'."),
      'field' => array(
        'handler' => 'views_handler_field_mcapi_state',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_mcapi_states',
      ),
    );

    $data['mcapi_transactions']['type'] = array(
      'title' => t('Type'),
      'help' => t('Which form or module which created the transaction'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_mcapi_types',
      ),
    );
    $data['mcapi_transactions']['creator'] = array(
      'title' => t('Creator'),
      'help' => t('The user who first created the transaction.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'label' => t('Creator'),
      ),
    );
    $data['mcapi_transactions']['created'] = array(
      'title' => t('Created'),
      'help' => t("The second the transaction was created."),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
      'argument' => array(
        'help' => t('Better to use the handler from date_views module'),
        'handler' => 'views_handler_argument_date',
        'field' => 'created',
      )
    );

    $data['mcapi_transactions']['operations'] = array(
      'title' => t('Operations'),
      'help' => t("What the user can do to the transaction"),
      'field' => array(
        'handler' => 'views_handler_field_transaction_operations',
        'additional fields' => array('serial')
      )
    );
    return $data;
  }
}
}

/*
 * implements hook_views_data_alter
 */
function mcapi_views_data_alter(&$data) {
  //this is a powerful way for virtual field balances to be accessible to views based on the user table
  //however it does require a query for each row which calls on a special API function
  $data['users']['current_balance'] = array(
    'title' => t('Transaction statistic'),
    'field' => array(
      'help' => t("The balance, trading volume, or transaction count for each user"),
      'handler' => 'views_handler_field_mcapi_users_totals',
    )
  );

  $currencies = currencies_load();
  if (count($currencies) > 1) {
    //override the default text handler put there for fieldAPI fields, so the filter shows a dropdown
    $data['field_data_worth']['worth_currcode']['filter']['handler'] = 'views_handler_filter_mcapi_currency';
  }
}


function transaction_access_callback_perm_transact_views($query, $condition, $state) {
  $condition->condition(1, user_access('transact'));
}
function transaction_access_callback_perm_manage_all_views($query, $condition, $state) {
  $condition->condition(1, user_access('manage all transactions'));
}
function transaction_access_callback_payee_views($query, $condition, $state) {
  $condition->condition('mcapi_transactions.payee', $GLOBALS['user']->uid);
}
function transaction_access_callback_payer_views($query, $condition, $state) {
  $condition->condition('mcapi_transactions.payer', $GLOBALS['user']->uid);
}
function transaction_access_callback_open_views($query, $condition, $state) {
  $condition->condition(1, 1);
}
function transaction_access_callback_creator_views($query, $condition, $state) {
  $condition->condition('mcapi_transactions.creator', $GLOBALS['user']->uid);
}

/*
 * implements hook_query_TAG_alter for views on the mcapi_transactions table
 * this modifies the query only to show the transactions visible to the user
 * according to currency view access settings per transaction state
 * this is pretty elaborate, but the access control has to be done in the query
 * or views pager won't know how many results there were
 */
function mcapi_query_transaction_view_access_alter(QueryAlterableInterface $query) {
  $query->addJoin('LEFT', 'field_data_worth', 'w', 'w.entity_id = mcapi_transactions.xid');return;
  $states = array_keys(mcapi_get_states());
  //what currencies can the current user see?
  $currencies = currencies_load($GLOBALS['user']);
  $or_currcodes = db_or();//where ((currcode = A and (blah)) OR ((currcode = B and (blah))
  foreach ($currencies as $currcode => $currency) {
    //where (currcode = A and (state = x) OR (state = Y) )
    $state_or_state = db_or();//where state = A OR B
    foreach ($states as $state) {
      $callbacks = array_filter($currency->view_transaction_states[$state]);
      foreach ($callbacks as $callback) {
        $callback .= '_views';
        $state_and_callback = db_and()->condition('mcapi_transactions.state', $state);
        $callback($query, $state_and_callback, $state);
        $state_or_state->condition($state_and_callback);
      }
    }
    $currcode_and_states = db_and()->condition('w.worth_currcode', $currcode)->condition($state_or_state);
    $or_currcodes->condition($currcode_and_states);
  }
  //to see this working, show the sql query on admin/structure/views/settings/basic
  $query->condition($or_currcodes);
}


/*
 * implements hook_form_FORM_ID_alter
 * renames the exposed filters on views forms
 */
function mcapi_form_views_exposed_form_alter(&$form, $form_state) {
  $description = t('Comma separated usernames');
  $form['payee']['#description'] = $description;
  $form['payer']['#description'] = $description;
}


/*
 * implements hook_views_pre_render
 * converts zero values display in transaction views
 */
function mcapi_views_pre_render(&$view) {
  if (in_array($view->base_table, array('mcapi_transactions', 'mcapi_index_views')))  {
    module_load_include('inc', 'mcapi');
    foreach($view->result as $rowid => $row) {
      if (!property_exists($row, 'field_worth')) return;
      foreach ($row->field_worth as $delta => $item) {
        worth_item_zero_adjustment($view->result[$rowid]->field_worth[$delta]['rendered']);
      }
    }
  }
}


/**
 * Implements hook_views_plugins().
 */
function mcapi_views_plugins() {
  return array(
    'module' => 'views',
    'access' => array(
      'curr' => array(
        'title' => t('Per currency access'),
        'help' => t("Visible to users with access to the (first) currency in the filter or contextual filter."),
        'handler' => 'views_plugin_access_currency',
        'uses options' => TRUE,
      ),
    )
  );
}
