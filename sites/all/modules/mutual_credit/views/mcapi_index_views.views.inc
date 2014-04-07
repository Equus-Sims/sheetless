<?php
//Id;

/*
 * Implements views hook_views_data
 * adds the currency code field to queries which is necessary for theming
 */
function mcapi_index_views_views_data() {
  $data = array();
  $data['mcapi_index']['table']['group'] = t('Transaction index');
  $data['mcapi_index']['table']['base'] = array(
//    'database' => variable_get('mcapi_db_read', 'default'),
    'field' => 'serial',
    'title' => t('Transaction totals'),
    'help' => t('Transaction totals'),
    'access query tag' => 'transaction_view_access_index',
    'defaults' => array('diff')
  );

  $data['mcapi_index']['xid'] = array(
    'title' => t('Transaction ID'), // The item it appears as on the UI,
    'help' => t('The Unique database key of the transaction'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['mcapi_index']['serial'] = array(
    'title' => t('Unique serial number'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The serial number of the transaction and dependents'),
      'handler' => 'views_handler_field_transaction',
      'additional fields' => array('serial'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['mcapi_index']['uid1'] = array(
    'title' => t('1st user'), // The item it appears as on the UI,
    'help' => t('The transactor, whose perspective we are looking from'),
    'field' => array(
      'handler' => 'views_handler_field_user_name',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid1',
      'label' => t('1st user'),
    ),
  );
  $data['mcapi_index']['uid2'] = array(
    'title' => t('2nd user'),
    'help' => t("The transactee"),
    'field' => array(
      'handler' => 'views_handler_field_user_name',
    ),
    //can't 'think of a use case for a filter here
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid2',
      'label' => t('2nd user'),
    ),
  );
  $data['mcapi_index']['type'] = array(
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
  $data['mcapi_index']['income'] = array(
    'title' => t('Income'), // The item it appears as on the UI,
    'help' => t('Number of units of currency incoming'),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_quantity',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'help' => t('transactions above or below a certain value'),
      'handler' => 'views_handler_filter_numeric'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mcapi_index']['expenditure'] = array(
    'title' => t('Expenditure'), // The item it appears as on the UI,
    'help' => t('Number of units of currency outgoing'),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_quantity',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'help' => t('transactions above or below a certain value'),
      'handler' => 'views_handler_filter_numeric'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mcapi_index']['diff'] = array(
    'title' => t('Change in balance'), // The item it appears as on the UI,
    'help' => t("Change in the users balance"),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_quantity',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_mcapi_index_diff'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mcapi_index']['volume'] = array(
    'title' => t('Volume'), // The item it appears as on the UI,
    'help' => t("Absolute value of the trade"),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_quantity',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mcapi_index']['balance'] = array(
    'title' => t('Balance'), // The item it appears as on the UI,
    'help' => t("Indexed running balance after each transaction. (1 database query per row)"),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_balance',
      'additional fields' => array('currcode', 'created')
    ),
  );
  $data['mcapi_index']['created'] = array(
    'title' => t('Created moment'),
    'help' => t('The moment the transaction was registered'),
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
      'fieldname' => 'created'
    ),
  );
  currencies_load();//because mcapi_init hasn't run yet
  $currencies = currencies_load();
  $data['mcapi_index']['currcode'] = array(
    'title' => t('Currency'),
    'help' => t("The currency ID"),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_currcode',
      'click sortable' => TRUE,
    ),
    'filter' => array(
       'handler' => 'views_handler_filter_mcapi_currency'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  
  $data['mcapi_index']['operations'] = array(
    'title' => t('Operations'),
    'help' => t("What the user can do to the transaction"),
    'field' => array(
      'handler' => 'views_handler_field_transaction_operations',
      'additional fields' => array('serial')
    )
  );
  $data['mcapi_index']['greco'] = array(
    'title' => t('Greco index'),
    'help' => t("System health metric designed by Tom Greco"),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_index_greco',
      'hide_empty' => TRUE,
    )
  );
  return $data;
}

/*
 * implements hook_views_data_alter
 * anything (field) that is joined to mcapi_transactions should be joined to mcapi_index also
 * except worth field, which is copied in the table already
 */
function mcapi_index_views_views_data_alter(&$tables) {

  foreach ($tables as $table => $info) {
    //ignore the fields we know about, this is just intended for fields added to transactions via field API
    if (in_array($table, array('field_data_worth', 'mcapi_signatures'))) continue;
    if (array_key_exists('table', $info) && array_key_exists('join', $info['table']) && array_key_exists('mcapi_transactions', $info['table']['join'])) {
      $tables[$table]['table']['join']['mcapi_index'] = $info['table']['join']['mcapi_transactions'];
      if (!array_key_exists('entity_id', $info)) continue;
      //can't remember what 'moved to' is for
      $fieldname = $info['entity_id']['field']['moved to'][1];
      if (is_array($info[$fieldname]['field']['entity_tables'])) {
        $tables[$table][$fieldname]['field']['entity_tables']['mcapi_index'] = 'transaction';
      }
    }
  }
  //tell the date_views module about this table
  $tables['mcapi_index']['date_argument'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => 'mcapi_index')),
    'help' => t('Filter any Views !base_table date field by a date argument, using any common ISO date/period format (i.e. YYYY, YYYY-MM, YYYY-MM-DD, YYYY-W99, YYYY-MM-DD--P3M, P90D, etc). ', array('!base_table' => 'mcapi_index')),
    'argument' => array(
      'handler' => 'date_views_argument_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    ),
  );
  // The flexible date filter.
  $tables['mcapi_index']['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => 'mcapi_index')),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'mcapi_index')),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    ),
  );
}


/**
 * Implements hook_views_plugins().
 * I'm not sure this is useful at all any more, or whether it works
 * in any case, this doesn't really belong in this module and will be deprecated
 */
function mcapi_index_views_views_plugins() {
  return array(
    'module' => 'views', // This just tells our themes are elsewhere.
    'access' => array(
      'arg1user' => array(
        'title' => t('Arg 1 user'),
        'help' => t('Access will be granted only to users is on their own profile pages - user/%'),
        'handler' => 'views_plugin_access_user_arg1',
        'uses options' => FALSE,
      ),
    )
  );
}


