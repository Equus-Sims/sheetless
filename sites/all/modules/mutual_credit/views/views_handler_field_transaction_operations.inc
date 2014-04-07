<?php
// $Id:

/**
 * @file
 * Field handler to show all the permitted transaction operations for a transaction
 * this should always be defined with
 *
 * additional fields => array('serial')
 */
class views_handler_field_transaction_operations extends views_handler_field_transaction {

  function render($values) {
    $transaction = transaction_load($values->{$this->aliases['serial']});
    $links = array();
    foreach (transaction_get_links($transaction, '', TRUE) as $link) {
      $links[] = drupal_render($link);
    }
    return drupal_render(transaction_get_links($transaction, FALSE, TRUE));
  }

}
