<?php
/**
 * @file
 * Formal description of transaction handling function and Entity controller functions
 *
 * N.B. transaction' can have 3 different meanings
 *  a database transaction (not relevant to this document)
 *  Fieldable entity with one or more '$items' each a in different currency. This is what views works with
 *  A transaction cluster is an array of the previous 'transactions',
 *  usually before they are written to the db, where they will have the same serial number and state
 *    The first transaction in a cluster 'volitional' and the rest, 'dependent',
 *   which means they were created automatically, from the volitional
 *   the dependent transactions share a serial number and state, but probably have a different 'type'
 *  When a transaction is loaded from the db, the dependents are put into (array)$transaction->dependents.
 */

/*
 * Community Accounting API FOR MODULE DEVELOPERS
 * WRAPPER FUNCTIONS
 * These 3 wrapper functions around the following transaction controller API
 * are mostly concerned with managing transactions as clusters sharing the same serial number
 * Module developers should use these functions wherever possible.
 */


/*
 * wrapper around entity_load
 * load a cluster of transactions sharing a serial number
 * The first transaction will be the 'volitional' transaction and the rest are loaded into
 * $transaction->dependents where the theme layer expects to find them
 * $serial can be varchar or an array of varchars
 */
stdClass transaction_load($serial);


/*
 * Entity API callback and wrapper around Community Accounting API function transaction_cluster_write
 * take one volitional transaction and allow the system to add dependents before sending the cluster to be written
 * Insert a single transaction entity, running the accounting_validate hook on it first
 * note that this includes a call to hook_transaction_post_insert
 */
try {
  array transaction_cluster_create($transaction, $really = TRUE);
}
catch(exception $e){}

/*
 * Insert a validated transaction cluster, and allocate a serial number to the cluster
 * N.B. Contrib modules would normally call wrapper function transaction_cluster_create()
 * @variable
 *  $transactions is a flat array
 * All $transactions will be given the same serial number
 * includes a call to hook_accounting_validate before writing
 * includes a call to hook_transaction_cluster_write
 */
try {
  transaction_cluster_write($transactions, $really);
}
catch(exception $e){}

/*
 * The default entity controller supports 3 undo modes
 * Utter delete
 * Change state to erased
 * Create counter-transaction and set state of both to TRANSACTION_STATE_UNDONE
 * NB this function goes on to call hook_transaction_undo()
 */
try {
  transaction_undo(serial);
}
catch(exception $e){}

/*
 * All changes to transactions should be passed through this
 * it saves the new state and field API and fires hooks and triggers wotnot
 * if the $old_state is set, that indicates this was a workflow operation
 * Calls hook_transaction_update
 */
try {
  transaction_update($newly_saved_transaction, $old_state);
}
catch(exception $e){}

/*
 * filter transactions
 * returns an array of serial numbers keyed by xid
 *
 * arguments, in any order can be
 * serial, integer or array
 * from // unixtime
 * to //unixtime
 * state integer or array
 * creator integer or array($uids)
 * payer integer or array($uids)
 * payee integer or array($uids)
 * involving integer or array($uids)
 * currcode string or array($currcodes)
 * type string or array($types)
 * no pager is provided in this function
 * views is much better
 * this hasn't been used yet
 */
$conditions = array('serial' => array('AB123', 'AB124'));
//or
$conditions = array('involving' => array(234, 567));

/*
 * this is a substitute for views
 * arguments:
 *  $conditions - an array of transaction properties, each with a value or array of values to filter for
 *  $offset - used for paging
 *  $limit - used for paging
 *  $fieldapi_conditions - more conditions for testing against loaded transactions.
 *   NB this could be expensive in memory
 *   NB paging is ignored
 *   NB in multiple cardinality fields, only the first value is filtered for
 */
array transaction_filter((array)$conditions, $offset, $limit, $fieldapi_conditions);



/*
 *
 * Retrieves transaction summary data for a user in a given currency
 *
 * This data can also be obtained through various views fields, especially in the mcapi_index_views module
 * $filters are same as in drupal database api, each an array like ($fieldname, $value, $operator),
 * applicable to the mcapi_transactions table (worth field is not supported)
 * where the fieldname is from mcapi_transactions table and the operator is optional.
 * If there are no conditions passed then only transactions in a positive STATE are counted.
 *
 * Returns an array with the following keys
 * - balance
 * - gross_in
 * - gross_out
 * - count
 */
array transaction_totals($uid, $currcode, $filters);


/*
 * list of hooks called in this module
 * no need to put hook_info coz that's just for lazy loading
 */

//declare new transaction controllers - only the function name is needed.
function hook_transaction_controller(){}

//check the transactions and the system integrity after the transactions would go through
//do NOT change the transaction
function hook_accounting_validate(array $transactions){}

//do your own writing for the transaction cluster
function hook_transaction_cluster_write(array $transactions){}


//respond to the creation of a transaction
function hook_transaction_post_insert($transaction){}

//preparing a transaction for rendering
function hook_transactions_view($transactions, $view_mode, $suppress_ops){}

//respond to the changing of a transaction
function hook_transaction_update($serial){}

//respond to the removal, or undoing of a transaction
function hook_transaction_undo($serial){}

//declare permissions for transaction access control, per currency per operation. See mcapi_transaction_access_callbacks
function hook_transaction_access_callbacks(){}


//declare transaction states
function hook_mcapi_info_states(){
  return array(
    99 => array(//ensure this number doesn't clash with existing states
      'name' => t('Rejected'),
      'description' => t('transaction was terminated by payee'),
      'default_access_callbacks' => array('mcapi_access_authenticated')//see hook_transaction_access_callbacks
    ),
  );
}
//declare transaction types, perhaps one for each workflow process
function hook_mcapi_info_types(){
  return array('donate', 'charge', 'rebate');
}

//declare permissions to go into the community accounting section of the drupal permissions page
function hook_mcapi_info_drupal_permissions(){}

//add transactions to a new cluster (this is not actually invoked with drupal_alter, but could be)
function hook_transaction_cluster_alter(){}



/**
 * Transaction operations
 * User stories are built up using transaction operations to make a workflow.
 * Operations allow transactions to be edited in a very controlled way.
 * They are declared in a hook, and shown to the user as a virtual transaction property, depending on access control
 * The following operations provided in mcapi.module are for internal use only (they have no access callback):
 * -transaction_register -> create
 * -transaction_field_update -> update
 * -transaction_deleted -> delete
 *
 * Each operation issues has a 'trigger' for user 1 or other modules to react.
 * See transaction_operation_form_submit for some insight.
 */

//things that can be done to transactions, typically changing state but also maybe editing fields.
function hook_transaction_operations(){
  return array(
    //the array key is a hook, so be careful with the namespace
    'undo' => array(
      //this is used for the MENU_LOCAL_ACTION
      //operations without a title are for internal use only
      'title' => "Undo",
      //a tooltop over the MENU_LOCAL_ACTION
      'description' => "Undo a finished transaction, and its dependents",
      //a message asking if the user is sure they want to do the operation
      'sure' => "Are you sure you want to undo? Only the site administrator will be able to restore this transaction.",
      //(optional) affects the order the operations are shown in.
      'weight' => 3,
      //function taking args $op, $transaction, $currency to see if the current user can do the op.
      'access callback' => 'mcapi_undo_access',
      //optional key to provide operation access settings in currency configuration
      'access form' => 'operations_config_default_access',//this is the default, can also be left blank
      //(optional), path to file, relative to the module root, containing form and submit callbacks
      'filepath' => 'mcapi.inc',
      //callback where the operation actually happens
      //it takes the ($operation, $transaction, $values) and returns a render array to replace the transaction
      'submit callback' => 'mcapi_undo',
      //(optional) inject any fields into the 'are you sure' page.
      //if it is empty or absent there will be NO CONFIRMATION STEP
      //if the callback name doesn't exist it won't break.
      //also a chance to drupal_set_title of that page (nojs mode only)
      'form callback' => 'TRUE',
      //this applies for the form, not for ajax. default will redirect to the transaction/$serial
      'redirect' => 'user'
    )
  );
  //Dont' forget to include these t()s
  t("Undo");
  t("Undo a finished transaction, and its dependents");
  t("Are you sure you want to undo? Only the site administrator will be able to restore this transaction.");
}

//alter hooks, more could be added, if necessary!
function hook_transaction_operations_alter(){}

/**
 * Note that more hooks are provided by entity API module for any transaction entity.
 */
