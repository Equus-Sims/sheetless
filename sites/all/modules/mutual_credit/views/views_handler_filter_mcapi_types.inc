<?php
// $Id: views_handler_filter_mcapi_types.inc,
/**
 * Filter handler for exchange types
 */
class views_handler_filter_mcapi_types extends views_handler_filter_in_operator {

  function get_value_options() {
    $this->value_options = drupal_map_assoc(mcapi_get_types());
  }
}