<?php

/*
 * callback for the currency edit form
 * when building the initial page, the $currency is passed, AJAX builds use form_state
 * When called via AJAX, the $settings_form should contain '#value' not '#default_value', not sure why
 */
function limits_settings_subform(&$form, $form_state, $currency) {
  //I'm not very happy about the way the ajax loads are populated by the previous value,
  //but I've been a couple of hours fumbling and we can live with it.
  //the default values are correct even in the form_alter, but on processing,
  //the min/maxfields seem to be populated with their previous value, regardless of the #default_value
  if (isset($form_state['values']['limits']['limits_callback'])) {
    $limits_callback = $form_state['values']['limits']['limits_callback'];
    $defaults = array();
  }
  elseif(isset($currency->limits['limits_callback'])) {
    $limits_callback = $currency->limits['limits_callback'];
    $defaults = isset($currency->limits[$limits_callback]) ? $currency->limits[$limits_callback] : array();
  }
  else {
    $limits_callback = 'limits_none';
    $defaults = array();
  }

  $form['limits'][$limits_callback] = array(
  		'#prefix' => '<div id="limits-settings">',
  		'#suffix' => '</div>',
  );

  $form_callback = $limits_callback .'_form';
  if (function_exists($form_callback)) {
    //get the fields
    $form['limits'][$limits_callback] += $form_callback($currency, $defaults);
  }
  //we return the array for the sake of the ajax callback but also the main form builder has been altered by reference
  return $form['limits'][$limits_callback];
}



/*
 * NO LIMITS
 */
function limits_none($currency, $uid) {
  return array();
}

/*
 * GLOBAL LIMITS
 */

//limits callback - returns the min an max limits for that currency for that user
function limits_global($currency, $uid) {
  return $currency->limits['limits_global'];
}
function limits_global_form($currency, array $defaults) {
  if (isset($currency->limits['limits_global'])) {
    $defaults += array_filter($currency->limits['limits_global']);
  }
  return min_max_fields($defaults);
}


/*
 * Equation
 * has tokens
 * @gross_in gross income
 * @gross_out gross spending
 * @balance number of trades
 * @trades number of trades
 * e.g. 10+@gross_in+@gross_out+@trades
 */
function limits_equations($currency, $uid) {
  $values = (array)transaction_totals($uid, $currency->info['currcode']);
  sort($values);
  $replacements = array_combine(
    array('@trades', '@gross_in', '@gross_out', '@balance', '@volume'),//sorted as declared
    $values
  );
  $max_formula = strtr($currency->limits['limits_equations']['max'], $replacements);
  $min_formula = strtr($currency->limits['limits_equations']['min'], $replacements);
  return array(
    'max' => eval('return $max_formula;'),
    'min' => eval('return $min_formula;'),
  );
}

function limits_equations_form($currency, array $defaults) {
  return array(
    'min' => array(
      '#title' => t('Formula to calculate minimum limit'),
      '#description' => t('Use the following tokens:') . ' '. t('@gross_in, @gross_out, @trades'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['min']) ? $defaults['min'] : '-',
      '#element_validate' => array('mcapi_limits_equation_validate'),
      '#required' => TRUE,
    ),
    'max' => array(
      '#title' => t('Formula to calculate maximum limit'),
      '#description' => t('Use the following tokens:') . ' '. t('@gross_in, @gross_out, @trades'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['max']) ? $defaults['max'] : '-',
      '#element_validate' => array('mcapi_limits_equation_validate'),
      '#required' => TRUE,
    ),
  );
}

//checks that the formula evaluates to a number
function mcapi_limits_equation_validate(&$element, &$form_state) {
  if (!strlen($element['#value'])) return;
  $value = mcapi_limits_equation_parse(
    $element['#value'],
    array('gross_in' => 100, 'gross_out' => 100, 'trades' => 10)
  );
  if (!is_numeric($value)) {
    form_error($element, t('Formula does not evaluate to a number: @result', array('@result' => $result)));
  }
}
function mcapi_limits_equation_parse($string, $values) {
  $pattern = 'return '. str_replace('@', '', $string) .';';
  $formula = strtr(
    $pattern,
    $values
  );
  return eval($formula);
}

function min_max_fields($defaults) {
  return array(
    'min' => array(
      '#title' => t('Optional minimum balance'),
      '#description' => t('Must be less than or equal to zero'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['min']) ? $defaults['min'] : '',
      '#element_validate' => array('mcapi_validate_optional_num', 'mcapi_limits_validate_min'),
      '#size' => 7,
      '#maxlength' => 7,
      '#attributes' => array('style' => 'width:inherit;')
     ),
     'max' => array(
      '#title' => t('Optional maximum balance'),
      '#description' => t('Must be greater than 0.'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['max']) ? $defaults['max'] : '',
      '#element_validate' => array('mcapi_validate_optional_num', 'mcapi_limits_validate_max'),
      '#size' => 6,
      '#maxlength' => 6,
      '#weight' => 1,
      '#attributes' => array('style' => 'width:inherit;'),
    )
  );
}

function mcapi_validate_optional_num(&$element, $form_state) {
  if ($element['#value'] == '-') {//this is the default field value, meaning minus zero
    form_set_value($element, 0, $form_state);
  }
  if (strlen($element['#value']) && !is_numeric($element['#value'])) {
    form_error($element, t('Limits must be numeric: @value', array('@value' => $element['#value'])));
  }
}

//validate callback subsection of currency form (and sometimes user profile form)
//this could be done in javascript also, on the form
function mcapi_limits_validate_min(&$element, $form_state) {
  if (!empty($element['#value']) && $element['#value'] > 0) {
    form_error($element, t('Minimum balance must not be larger than 0'));
  }
}

//validate callback subsection of currency form (and sometimes user profile form)
//this could be done in javascript also, on the form
function mcapi_limits_validate_max(&$element, $form_state) {
  if (!empty($element['#value']) && $element['#value'] <= 0) {
    form_error($element, t('Maximum balance must be larger than 0'));
  }
}


function _mcapi_limits_form_ctools_export_ui_edit_item_form_alter(&$form, &$form_state) {
  if (!currencies_filter(CURRENCY_TYPE_EXCHANGE, CURRENCY_TYPE_COMMODITY)) {
    //couldn't be bothered to enforce this
    //drupal_set_message('Balance limits module should be disabled while only currencies of acknowledgement are present.');
  }
  $currency = isset($form_state['values']) ? (object)$form_state['values'] : $form_state['item']->data;
  //of course this is meaningless if we are creating the currency for the first time

  //limits could be managed by drupal OR the entity controller. Drupal offers more flexibility
  $warning[] = t("With great power comes great responsibility!");
  $warning[] = t("The first rule of accounting is: no account should issue more credit than it can redeem.");
  $warning[] = t("Even fiat currencies can be run sustainably if they are used to pay tax.");
  $warning[] = t("Trust in the system can be increased by limiting the right to issue credit to trusted, known, productive actors in the economy, which may or not include the goverment!");
  $warning[] = t("Transactions will be rejected which take accounts beyond these limits.");
  $warning[] = t("Choose which method to use, and configure it.");
  $form['limits'] = array(
    '#title' => t('Balance limits'),
    '#description' => implode(' ', $warning),
    '#type' => 'fieldset',
    '#weight' => 20,
    '#group' => 'additional_settings',
    '#collapsible' => TRUE,
    '#collapsed' => isset($form['#currency']->limits_callback),
    //'#group' => 'additional_settings',
    'limits_callback' => array(
      '#title' => t('Method'),
      '#description' => t('How will the limits be determined?'),
      '#type' => 'select',
      '#options' => module_invoke_all('mcapi_limits_info'),
      '#default_value' => isset($currency->limits['limits_callback']) ? $currency->limits['limits_callback'] : 'limits_none',
      '#weight' => -1,
      '#ajax' => array(
        'callback' => 'limits_settings_subform',
        'wrapper' => 'limits-settings',
      ),
    ),
  );
  limits_settings_subform($form, $form_state, $currency);
  //we are relying in the inserted fields to validate themselves individually, so there is no validation added at the form level
  $form['limits']['personal'] = array(
    '#title' => t('Personal limits'),
    '#description' => t('Settings on the user profiles override these general limits.'),
    '#type' => 'checkbox',
    '#default_value' => isset($currency->limits['personal']) ? $currency->limits['personal'] : 0,
    '#weight' => 5,
  	'#states' => array(
  	  'invisible' => array(
  		  ':input[name="limits[limits_callback]"]' => array('value' => 'limits_none')
  	  )
  	)
  );
  $form['limits']['skip'] = array(
    '#title' => t('Skip balance limit check'),
    '#description' => t('Especially useful for mass transactions and automated transactions'),
    '#type' => 'checkboxes',
    '#default_value' => isset($currency->limits['skip']) ? $currency->limits['skip'] : array(),
    //would be nice if this was pluggable, but not needed for the foreseeable
    '#options' => array(
      'auto' => t("for transactions of type 'auto'"),
      'mass' => t("for transactions of type 'mass'"),
      'user1' => t("for transactions created by user 1"),
      'owner' => t("for transactions created by the currency owner"),
      'reservoir' => t("for the reservoir account")
    ),
  	'#states' => array(
  	  'invisible' => array(
  		  ':input[name="limits[limits_callback]"]' => array('value' => 'limits_none')
  	  )
  	),
    '#weight' => 6,
  );
}
