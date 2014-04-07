<?php
// $Id:

/**
 * @file
 * Field handler to retrieve the running balance by adding the previous transactions
 * NB this doesn't apply to the worth_field and mcapi_transactions table, only to the transaction_index table
 * Unlike conventional accounting, in which each transaction is written indelibly into a log,
 * this system allows backdated transactions, which means the xid / serial number is no guarantee of chronology.
 * The only chronology we have therefore is the transaction->created date.
 * Since the running balance isn't stored in the virtual table, so can't be looked up by views,
 * this handler must calculate the running balance for every row, by looking at all the previous rows, cronologically
 * If transactions were created in the same second, this may produce unusual results
 * However the transaction->insert method tries to ensure that no user has a transaction in the same second
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_mcapi_index_balance extends views_handler_field {

  var $uid;

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $currcode = &$values->{$this->aliases['currcode']};
    if (empty($this->uid)) {
      $arg = array_search('uid1', array_keys($this->view->argument));
      if (is_numeric($arg)) {
        $this->uid = $this->view->args[$arg];
      }
      elseif (isset($this->view->filter['uid1'])) {
        $this->uid = $this->view->filter['uid1']->value['value'];
      }
      else {
        drupal_set_message("Running balance requires filter or contextual filter 'Transaction index: 1st user'", 'error', FALSE);
        return 'error';
      }
    }
    //the running balance depends the order of the transactions
    //order or creation is not enough if they were created at the same time
    //created date is not necessarily unique per transaction, so needs a secondary sort.

    return array(
      '#theme' => 'worth_item',
      '#currcode' => $currcode,
      '#quantity' => db_query(
        "SELECT SUM(diff) FROM {mcapi_index}
          WHERE uid1 = $this->uid
          AND created <= :created
          AND currcode = '$currcode'",
        array(':created' => $values->{$this->aliases['created']})
      )->fetchField()
    );
  }
}