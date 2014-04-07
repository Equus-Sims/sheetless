<?php
// $Id:

/**
 * @file
 */
class views_handler_field_mcapi_index_greco extends views_handler_field {

  var $currcode_alias;

  function query() {
    $this->ensure_my_table();
    mcapi_views_ensure_currcode($this->query, $this->currcode_alias);
  }


  function render($values) {
    $data = variable_get('mcapi_greco_system_health', array());
    if (!array_key_exists($values->{$this->currcode_alias}, $data)) return array();
    $render = array();
    if (module_exists('advanced_help')) {
      $adv = array(
        '#theme' => 'advanced_help_topic',
        '#module' => 'mcapi_index_views',
        '#topic' => 'greco'
      );
      $render[]['#markup'] = drupal_render($adv);
    }
    $render[]['#markup'] = $data[$values->{$this->currcode_alias}];

    return $render;
  }
}
