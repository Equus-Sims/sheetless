<?php
//Id;

class views_handler_field_mcapi_state extends views_handler_field {

  private $states;

  function __construct() {
    if (isset($this->options['state_name']) && $this->options['state_name']) {
      $this->states = mcapi_get_states('#options');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['state_name'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['state_name'] = array(
      '#title' => t('Use state name'),
      '#description' => 'Or else use the raw integer value',
      '#type' => 'checkbox',
      '#default_value' => $this->options['state_name']
    );
  }

  function render($values) {
    $raw = $this->get_value($values);
    if ($this->options['state_name']) return $this->states[$raw];
    else return $raw;
  }
}
