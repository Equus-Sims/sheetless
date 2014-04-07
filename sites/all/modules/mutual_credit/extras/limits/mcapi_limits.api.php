<?php 

/*
 * implements hook_mcapi_limits_info
 */
function MYMODULE_mcapi_limits_info() {
  return array('my_callback' => t('My method name'));
}

/*
 * Your callback to determine the limits
 * Params
 * * Currency (object) derived from currency_load($currcode)
 * * $uid (integer)
 * Returns
 * an array with keys min and/or max with numeric values or NULL for each
 */
function my_callback($currency, $uid) {
  return array(
    'max' => 1000
  );
}


/*
 * Optional callback to configure the limits on the currency form
 * Made from my_callback + _form
 * Params
 * * Currency (object) derived from currency_load($currcode)
 * * $defaults, the saved values
 * Returns
 * a formAPI array
 */
function my_callback_form($currency, array $defaults) {
  $form = array(
    '#title' => 'Age beyond which credit is not granted',
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive')
    '#default_value' => $defaults
  );
  return $form;
}

