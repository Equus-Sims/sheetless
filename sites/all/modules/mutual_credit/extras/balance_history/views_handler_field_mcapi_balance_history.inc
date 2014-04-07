<?php
// $Id:

/**
 * @file
 */
class views_handler_field_mcapi_balance_history extends views_handler_field {


  function option_definition() {
    $options = parent::option_definition();
    $options['width'] = array('default' => 300);
    $options['height'] = array('default' => 100);
    $options['since'] = array('default' => 0);
    return $options;
  }


  function options_form(&$form, &$form_state) {
    $form['width'] = array(
    	'#title' => t('Pixel width'),
      '#type' => 'textfield',
      '#validate' => array('element_validate_integer_positive'),
      '#default_value' => $this->options['width'],
      '#required' => TRUE
    );
    $form['height'] = array(
    	'#title' => t('Pixel height'),
      '#type' => 'textfield',
      '#validate' => array('element_validate_integer_positive'),
      '#default_value' => $this->options['height'],
      '#required' => TRUE
    );
    $form['since'] = array(
      '#title' => t('Since'),
      '#description' => "a string which can be parsed by php's !strtotime",
      '#type' => 'textfield',
      '#validate' => array(), //TODO?
      '#default_value' => $this->options['since']
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }


  function render($values) {
    //DO NOT RENDER IF ITS A VIEWS PREVIEW
    if (property_exists($this->view, 'live_preview')) {
      return 'no preview available';
    }
    $account = user_load($values->{$this->aliases['uid1']});
    $renderable = array(
      '#theme' => 'balance_history',
      '#account' => $account,
      '#histories' => get_balance_history($account),
      '#width' => $this->options['width'],
      '#height' => $this->options['height']
    );
    return drupal_render($renderable);
  }
}
