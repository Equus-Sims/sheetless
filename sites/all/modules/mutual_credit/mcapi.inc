<?php
// $Id: mcapi.inc,v 1.1.2.7 2010/12/22 19:30:29 matslats Exp $

$controller_mod = variable_get('mcapi_controller', 'mcapi');
require_once(drupal_get_path('module', $controller_mod) .'/'. $controller_mod).'.controller';

//since the entity module was integrated after this module was deployed
//I can't seem to remove this if statement making upgrades extremely difficult
if (class_exists('Entity')) {
class transaction extends Entity {
  protected function defaultLabel() {
    return $this->title;
  }

  public function identifier() {
    return $this->serial;
  }

  protected function defaultUri() {
    return array('path' => 'transaction/' . $this->serial);
  }
}
}


/*
 * transaction entity definition 'label callback'
 */
function transaction_label($transaction){
  return t('Transaction #@serial', array('@serial' => $transaction->serial));
}
/*
 * transaction entity definition 'uri callback'
 */
function transaction_uri($transaction) {
  return array(
    'path' => 'transaction/' . $transaction->serial,
  );
}

/**
 * All transaction forms should use this function as a basis using hook_forms
 */
function _transaction_form(&$form, &$form_state) {
  //the default payer and payee widgets will autocomplete any user on the system, and permissions will be checked later
  $form['payer'] = array(
    '#title' => t('Account to be debited'),
    '#type' => 'user_chooser_few',
    '#callback' => 'user_chooser_segment_perms',
    '#args' => array('transact'),
    '#default_value' => $form['#transaction']->payer,
    '#weight' => 6,
  );
  $form['payee'] = array(
    '#title' => t('Account to be credited'),
    '#type' => 'user_chooser_few',
    '#callback' => 'user_chooser_segment_perms',
    '#args' => array('transact'),
    '#default_value' => $form['#transaction']->payee,
    '#weight' => 9,
  );
  $form['type'] = array(
    '#title' => t('Transaction type'),
    '#options' => drupal_map_assoc(module_invoke_all('mcapi_info_types')),
    '#type' => 'select',
    '#default_value' => $form['#transaction']->type,
    '#element_validate' => array('mcapi_validate_ttype'),
    '#required' => TRUE,
    '#weight' => 15
  );
  $form['state'] = array(
    '#title' => t('State'),
    '#description' => mcapi_get_states('#description'),
    '#type' => 'radios',
    '#options' => mcapi_get_states('#options'),
    '#default_value' => $form['#transaction']->state,
    '#element_validate' => array('mcapi_validate_state'),
    '#weight' => 18
  );
  $form['creator'] = array(
    '#title' => t('Recorded by'),
    '#type' => 'user_chooser_segment_perms',
    '#default_value' => $form['#transaction']->creator,
    '#args' => array('transact'),
    '#required' => TRUE,
    '#weight' => 20,
  );

  field_attach_form('transaction', $form['#transaction'], $form, $form_state);

  $form['buttons'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Record'),
      //this prevents double click, but doesn't prevent going back and resubmitting the form
      //'#attributes' => array('onclick' => "this.disabled=true,this.form.submit();"),
    ),
    '#weight' => 25
  );
}

/*
 * standard form-level validation callback for ALL transaction forms
 * fields have already been validated individually
 * By this time each of the form values should be well enough validated not to fail on writing to db
 */
function transaction_form_validate($form, &$form_state) {
  $props = $form_state['values'];

  //entity_form_submit_build_entity('transaction', NULL, $form, $form_state);
  $transaction = entity_create('transaction', $props);
  if ($transaction->payer == 0 || $transaction->payee == 0) {
    form_set_error('Cannote trade with user 0. This is a known error! please mail matslats@gmail.com and say what exactly you tried to do! ');
  }
  //validate the Field API fields
  field_attach_form_validate('transaction', $transaction, $form, $form_state);
  //finish building the transaction entity from form_state and validate that
  //Actually I don't believe this does anything we need
  //entity_form_submit_build_entity('transaction', $transaction, $form, $form_state);
  if (form_get_errors()) return;
  try{
    //add the dependent transactions, validate, test write and put the checked cluster in form_state
    //might have used entity_save here, but it doesn't return the value we need
    //these transactions are stored in the form to be used for a preview perhaps in step 2.
    $form_state['transactions'] = transaction_cluster_create($transaction, FALSE);
  }
  catch (Exception $e) {
    form_set_error('', $e->getMessage());
  }
}


/*
 * form submit callback from the base transaction form, hence USED BY ALL forms
 * this is what actually saves the data
 */
function transaction_form_submit($form, &$form_state) {
  $form_build_id = $form_state['values']['form_build_id'];
  //check the form hasn't been submitted already
  if(db_query(
    'SELECT count(form_build_id) FROM {mcapi_submitted} where form_build_id = :id',
    array(':id' => $form_build_id)
  )->fetchField() ) {
    drupal_set_message(t('Transaction was already submitted'), 'error');
    return;
  }
  //ensure that the form won't be submitted again
  db_query('INSERT INTO {mcapi_submitted} (form_build_id, time) VALUES (:form_build_id, :time)',
    array(':form_build_id' => $form_build_id, ':time' => REQUEST_TIME)
  );
  form_state_values_clean($form_state);
  $transaction = entity_create('transaction', $form_state['values']);
  //build the transaction object from the $form_state[values]
  entity_form_submit_build_entity('transaction', $transaction, $form, $form_state);
  try{
    transaction_cluster_create($transaction, TRUE);
  }
  catch (Exception $e) {
    form_set_error('', $e->getMessage());
  }
  $form_state['values']['serial'] = $transaction->serial;
}

/*
 * form element validation callback functions
 */
function mcapi_validate_title(&$element, $form_state) {
  //don't know the max length of 'small' text field. Will be just truncated
  $form_state['values']['title'] = check_plain($element['#value']);
}

/*
 * Validate callback for transaction form 'state' element
 * as a form element validation callback, this is assumed to happen BEFORE transactions_validate is called
 */
function mcapi_validate_state(&$element, &$form_state) {
  $states = mcapi_get_states('#full');
  if (!isset($states[$element['#value']])) {
    form_error($element, t('Invalid transaction state: @value.', array('@value' => $element['@value'])));
  }
}
/*
 * menu page callback
 * builds 'are you sure' operations form for both ajax and nojs
 */
function mcapi_get_op_form($op, $transaction, $mode = 'nojs') {
  drupal_add_js('misc/ajax.js', array('type' => 'file'));
  $operations = transaction_operations();
  if (array_key_exists('form callback', $operations[$op])) {
    $form = drupal_get_form('transaction_operation_form', $op, $transaction);
    //this form will sumit and expect ajax
    $form['actions']['submit']['#attributes']['class'][] = 'use-ajax-submit';
    drupal_add_js('misc/jquery.form.js');
    drupal_add_js('misc/ajax.js', array('type' => 'file'));
  }
  //return normally, or via ajax if called via ajax
  if ($mode == 'nojs') {//return a render array for delivery as a page
    //return an 'are you sure form'
    if (isset($form)) {
      return $form;
    }
    //do the operation directly, without a form, and return the render array
    else {
      return mcapi_submit_op_form($operations[$op], $transaction);
    }
  }
  elseif ($mode == 'ajax') {//return ajax commands
    //when the button is clicked replace the whole transaction div with the results.
    $commands[] = ajax_command_replace('#transaction-'.$transaction->serial, drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands
    );
  }

}

/*
 * default form callback for all transaction operations
 */
function transaction_operation_form($form, &$form_state, $op, $transaction) {
  $operations = transaction_operations(TRUE, FALSE);
  $info = $operations[$op];
  mcapi_hide_tabs();
  $form['serial'] = array(
    '#type' => 'hidden',
    '#value' => $transaction->serial
  );
  $form_state['transaction_operation'] = $operations[$op]; //pass the op to the submit handler
  if (array_key_exists('form callback', $info) && function_exists($info['form callback'])) {
    $form += $info['form callback']($op, $transaction);
  }
  $form['certificate'] = transaction_view($transaction, 'certificate', TRUE);
  //we might want to write something a bit cleaner than this
  $form = confirm_form(
    $form,
    entity_label('transaction', $transaction),//this page title stays in place throughout the ajax operation
    isset($info['redirect']) ? $info['redirect'] : 'transaction/'. $transaction->serial,
    $info['sure'],
    $info['title'],
    t('Not so sure'),
    $op
  );
  return $form;
}
//this is only called from the are you sure form, and always from ajax
function transaction_operation_form_submit($form, &$form_state) {
  $transaction = transaction_load($form_state['values']['serial']);
  $renderable = mcapi_submit_op_form($form_state['transaction_operation'], $transaction, $form_state['values']);
  //if this is ajax we return the result, otherwise redirect the form
  $commands[] = ajax_command_replace('#transaction-operation-form', drupal_render($renderable));
  ajax_deliver(array(
    '#type' => 'ajax',
    '#commands' => $commands
  ));
  exit();
}

//this is called in operation form submission OR ajax
function mcapi_submit_op_form($operation, $transaction, $values = array()) {
  $result = array();
  $old_state = $transaction->state;
  $function = $operation['submit callback'];
  //this changes the $transaction
  $result = $function($operation, $transaction, $values) or 'operation returned nothing renderable';
  //and invoke trigger and rules using the changed transaction
  transaction_update($transaction, $old_state);
  return $result;
}

/*
 *  access callback for transaction operation 'view'
 */
function mcapi_view_access($op, $transaction, $currency) {
  return _transaction_check_access_callbacks($currency->view_transaction_states[$transaction->state], $transaction);
}

/*
 *  access callback for transaction operation 'undo'
 * Undo is the only 'exposed' operation in this module
 */
function transaction_op_undo_access($op, $transaction, $currency) {
  if ($transaction->state != TRANSACTION_STATE_FINISHED) return FALSE;
  if ($transaction->type == 'remote') return FALSE;//otherwise the intertrading module has to somehow intervene
  return _transaction_check_access_callbacks($currency->access_operations[$op], $transaction);
}
function transaction_op_undo_form($op, $transaction) {
  return array();
}
function transaction_op_undo($operation, $transaction, $values) {
  transaction_undo($transaction->serial);
  return array('#markup' => t('The transaction is undone.') .' ');
}
/*
 * helper function to run through all the access callbacks after the operation's own access callback has done its tests
 */
function _transaction_check_access_callbacks($callbacks, $transaction) {
  foreach (array_filter($callbacks) as $function) {
    if ($function($transaction)) return TRUE;
  }
  return FALSE;
}

/*
 * transaction_access callbacks
 * $ops are determined in hook_transaction_operations
 */
function transaction_access_callback_perm_transact() {
  return user_access('transact');
}
function transaction_access_callback_perm_manage_all() {
  return user_access('manage all transactions');
}
function transaction_access_callback_payee($transaction) {
  return $GLOBALS['user']->uid == $transaction->payee;
}
function transaction_access_callback_payer($transaction) {
  return $GLOBALS['user']->uid == $transaction->payer;
}
function transaction_access_callback_open() {
  return TRUE;
}
function transaction_access_callback_creator($transaction) {
  return $GLOBALS['user']->uid == $transaction->creator;
}

/*
 * action callback
 * mail the transaction participants, and cc
 * NB Ignored here, but note that $transaction may have ->dependents property
 * with other transactions and other payees / payers
 */
function mcapi_transaction_mail_action($transaction, $settings, $old_state) {
  //the actions settings allow to filter by transaction type and state
  if ($transaction->state != $settings['state']) return;
  if (empty($settings['transaction_types'][$transaction->type])) return;

  $params = $settings + array('transaction' => $transaction);
  foreach (array($transaction->payer, $transaction->payee) as $uid) {
    if ($GLOBALS['user']->uid != $uid) {
      $params['user'] = user_load($uid);
      drupal_mail('mcapi', 'transaction_mail_action', $params['user']->mail, user_preferred_language($params['user']), $params);
    }
  }
}

//this should really be in core...
function _mcapi_validate_mail_element($element, $form_state) {
  if ($element['#value'] && !valid_email_address($element['#value'])) {
    form_error($element, t('That e-mail address is not valid.'));
  }
}

/*
 * action callback for hook_transaction_cluster_alter
 * Alters the original transaction and/or returns a dependent
 */
function mcapi_dependent_transaction_action($transaction, $settings) {
  if (!in_array($transaction->type, $settings['conditions']['transaction_types'])) return;
  $child = array(
    'type' => 'dependent',
    'state' => $transaction->state
  );

  foreach ($settings['presets']['worth_items']['worths'] as $delta => $item) {
    //if this currency was in the prime transaction, pass it to the calculator
    $calculated = _mcapi_calc($item['quantity'], @$transaction->worth[LANGUAGE_NONE][$delta]['quantity']);
    if (!$calculated) continue;
    $rounded = mcapi_round($calculated, $item['currcode'], $settings['presets']['worth_items']['round'] == 'up');
    //don't save zero value auto transactions, even if the currency settings permit
    if ($rounded == 0) continue;
    //if a quant was returned (and there really should be, from at least one currency), add it to the $dependent
    $child['worth'][$delta] = array(
      'quantity' => $rounded,
      'currcode' => $item['currcode']
    );
  }
  if (!isset($child['worth'])) return;

  $otheruser = &$settings['presets']['participants']['otheruser'];
  switch ($settings['presets']['participants']['mapping']) {
    case 'payerpays':
      $child['payee'] = $otheruser;
      $child['payer'] = $transaction->payer;
      break;
    case 'payeepays':
      $child['payee'] = $otheruser;
      $child['payer'] = $transaction->payee;
      break;
    case 'payspayer':
      $child['payee'] = $transaction->payer;
      $child['payer'] = $otheruser;
      break;
    case 'payspayee':
      $child['payee'] = $transaction->payee;
      $child['payer'] = $otheruser;
      break;
  }
  $child['description'] = @$settings['presets']['description'];
  return entity_create('transaction', $child);
}


/*
 * calculate a transaction quantity based on a provided formala and input quantity
 * $formula can be
 * number => returns the number as a quantity, regardless of $base_quant
 * percentage of the $base_quant
 * formula using [q] for base_quant
 */
function _mcapi_calc($formula, $base_value) {
  if (is_numeric($formula)) return $formula;
  $proportion = str_replace('%', '', $formula);
  if (empty($base_value)) $base_quant = 0;
  if (is_numeric($proportion)) {
    return $base_value * $proportion/100;
  }
  //$formula = str_replace('/', '//', $formula);
  $equation = str_replace('[q]', $base_value, $formula) .';';
  $val = eval('return '. $equation);
  if (is_numeric($val)) return $val;
  drupal_set_message(t('Problem with calculation for dependent transaction: @val', array('@val' => $val)));
}
//TODO
//Parse error: syntax error, unexpected '/' in /var/www/drupal7/sites/all/modules/matslats/mutual_credit/mcapi.inc(853) : eval()'d code on line 1


/*
 * helper to get block settings and load whichever user we like
 * may return nothing if no user is determined
 * the possible values of user_source should be made into php constants
 * 0 //the user argument comes from the current path
 * 1 //the user argument is the current user
 */
function _mcapiblock_settings($delta, $currency_access_mode) {
  $settings = variable_get('mcapiblock_'. $delta, array('currcodes' => array(), 'user_source' => 0));
  if (@$settings['user_source'] == 0) {//Show as part of profile being viewed
    if (arg(0) == 'user') {//and we are viewing a profile...
      $settings['account'] = user_uid_optional_load(arg(1));
    }
    else return;
  }
  else {
    $settings['account'] = $GLOBALS['user'];
  }
  if (empty($settings['currcodes'])) {
    $settings['currcodes'] = array_keys(currencies_load($settings['account']));
  }
  //check that the current user can see the aggregated trading data in this currency
  foreach($settings['currcodes'] as $key => $currcode) {
    if (!currency_access($currency_access_mode, $currcode)) unset($settings['currcodes'][$key]);
  }
  if (empty($settings['currcodes']))return;

  return $settings;
}


/*
 * Calculate the balance changes that this transaction proposes
 * by convention, if the transaction state < 0 it is NOT COUNTED
 * this is only used in tokens, so far, and in mcapi_limits module
 * incoming transaction can be a transaction object with children or an array
 */
function _transactions_diff($transaction) {
  $diff = array();
  //because this can be called before or after the transaction,
  //it might come in as a cluster, or as an object with dependents
  //we need a cluster so we can iterate though
  $cluster = is_array($transaction) ? $transaction :  array_merge(array($transaction) + (array)$transaction->dependents);
  foreach ($cluster as $transfer) {
    foreach ($transfer->worth[LANGUAGE_NONE] as $item) {
      $quant = $transfer->state < -1 ? 0 : $item['quantity'];//i is signature module is enabled, pending transactions count here
      //we can't prepare the array in advance with zeros so += and -= throws notices
      //instead we just build up an array and add them up when we need them
      $diff[$transfer->payer][$item['currcode']][] = -$quant;
      $diff[$transfer->payee][$item['currcode']][] = $quant;
    }
  }
  return $diff;
}

/*
 * theme implementation of template_preprocess_HOOK
 * make the transaction variables into something which can be nicely themed
 * $variables['transaction'] may have ->dependents from transaction_load
 * or it may be a simulated transaction with no xid and serial or dependents
 */
function template_preprocess_certificate(&$vars) {
  $transaction = &$vars['object'];
  $vars['payer'] = theme('username', array('account' => user_load($transaction->payer)));
  $vars['payee'] = theme('username', array('account' => user_load($transaction->payee)));
  $vars['state'] = $transaction->state;
  $vars['recorded'] = format_date($transaction->created, 'medium');
  $vars['type'] = $transaction->type;
  $vars['date'] = format_date($transaction->created, 'long');
  $vars['classes_array'] = array('transaction', 'certificate', $transaction->type, 'state-'.$transaction->state);
  if (!property_exists($transaction, 'xid') || empty($transaction->xid)) {
    $vars['classes_array'][] = 'preview';
    foreach (array_keys(field_info_instances('transaction', 'transaction')) as $fieldname) {
      //remove all fieldAPI fields because the entity isn't in the database yet and doesn't have an entity_id
      if (!in_array($fieldname, array('worth', variable_get('transaction_description_field', '')))) {
        if (property_exists($transaction, $fieldname)) unset($transaction->$fieldname);
      }
    }
  }

  //this returns a render array
  $vars['additional'] = field_attach_view('transaction', $transaction, 'certificate');
  //this populates top level of variables with the render array
  field_attach_preprocess('transaction', $transaction, $vars['additional'], $vars);

  foreach (element_children($vars['additional']['worth']) as $delta) {
    worth_item_zero_adjustment($vars['additional']['worth'][$delta]);
  }
  //move worth field to the top of $vars where we would expect to find it
  $vars['worth'] = drupal_render($vars['additional']['worth']);
  unset($vars['additional']['worth']);
  //choose the transaction theme template based on the first currency of the transaction
  $first_item = reset($transaction->worth[LANGUAGE_NONE]);
  $vars['theme_hook_suggestions'][] = 'certificate__' . $first_item['currcode'];
  $vars['theme_hook_suggestions'][] = 'certificate__' . $transaction->type;
  if (isset($transaction->dependents) && !empty($transaction->dependents)) {
    $vars['dependents'] = transaction_view(
      $transaction->dependents,
      str_replace('[transaction:links]', '', variable_get('mcapi_sentence', mcapi_sentence_default()))
    );
  }
}

//this will adjust the rendering of zero worth in accordance with the currency display zero property
//it is not used every time a 'worth' is rendered, but only when transactions are displayed.
//for this reason, though it seems inelegant, this function should be called from wherever the transaction is rendered
function worth_item_zero_adjustment(&$worth_render_array) {
  $currencies = currencies_load();
  if ($worth_render_array['#quantity'] == 0) {
    $worth_render_array = array('#markup' => $currencies[$worth_render_array['#currcode']]->display['zero']);
  }
}

/**
 * element validation callback
 * check the centiles value is valid
 * check that the $quantity is a number
 */
function worth_element_validate(&$element, &$form_state, $form) {
  if (!isset($element['#value']['quantity'])) return;
  $currency = currency_load($element['#value']['currcode']);
  if (!$currency) {
    form_error($element, t('This currency does not exist: @currcode', array('@currcode' => $element['#value']['currcode'])));
  }
  $quant = &$element['#value']['quantity'];
  if (strlen($quant) && !is_numeric($quant) && !$element['#calc']) {
    form_error($element, t('@fieldname must be numeric', array('@fieldname' => $element['#title'])));
  }
  if ($quant < 0) {
    form_error($element, t('Negative values not allowed'));
  }

  if ($division = $quant - intval($quant)) {
    //check that divisions match allowed values for the currency
    if (count($currency->display['divisions_allowed']) && !isset($currency->display['divisions_allowed'][$division*100])) {
      form_error($element, t('@division is not a permitted division of currency @name', array('@division' => $division, '@name' => $currency->human_name)));
    }
  }
  if (array_key_exists('#max', $element) && $element['#max'] && $element['#value']['quantity'] > $element['#max']) {
    form_error($element, t('Maximum value exceeded for !currency', array('!currency' => $currency->human_name)));
  }
}

/*
 * default worths element validator
 * and check required
 * clear the $items which are empty
 */
function worths_element_validate(&$element, &$form_state) {
  //we need to drop the empty values to prevent them being written to the database
  //unless the currency settings explicitly allow it.
  //first of all set the form_State value, which could not be set from the form_type_worths_value
  //because of the way it is populated from the container inwards
  drupal_array_set_nested_value($form_state['values'], $element['#array_parents'], $element['#value']);
  if ($element['#required']) {//note that this checks all the $items together
    if (empty($element['#value'])) {
      form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
    }
  }
}


function _worths_element_process($element, &$form_state) {
  $currencies = currencies_load();
  //the default value also tells us which currencies are available
  //if no defaults are set then all currencies should be present
  if (!isset($element['#default_value']) || empty($element['#default_value']) || @$element['#preset']) {
    $element['#default_value'] = (array)$element['#default_value'];
    foreach ($currencies as $currcode => $currency) {
      $element['#default_value'] += array($currcode => '');
    }
  }
  $delta = 0;
  if (variable_get('mcapi_mixed_transactions', TRUE) || @$element['#preset']) {
    //construct one widget for each default, each with its own currcode
    foreach($element['#default_value'] as $currcode => $quant) {
      $element[$delta]['#title'] = $currencies[$currcode]->human_name;
      $element[$delta]['#currcodes']= array($currcode);
      $element[$delta]['#default_value'] = array(
        'currcode' => $currcode,
        'quantity' => $quant
      );
      $delta++;
    }
  }
  else {
    $element[0]['#title'] = t('Worth');
    foreach($element['#default_value'] as $currcode => $quant) {
      $element[0]['#currcodes'][] = $currcode;
      $element[0]['#default_value']['currcode'] = $currcode;
      if (strlen($quant)) {
        $element[0]['#default_value']['quantity'] = $quant;
      }
    }
    if (array_key_exists('#max', $element) && $element['#max']) {
      $element['#description'] = $element['#description'] . t('Max: !num', array('!num' => $element['#max']));
    }
  }
  //now populate all the subwidgets with defaults
  foreach (element_children($element) as $delta) {
    $element[$delta]['#type'] = 'worth';
    $element[$delta]['#calc'] = @$element['#calc'];
    $element[$delta]['#preset'] = @$element['#preset'];
  }
  return $element;
}

/*
 * implements form_type_ELEMENT_NAME_value
 * default value_callback for worths
 * this function is included because the worths process callback includes mcapi.inc
 */
function form_type_worths_value(&$element, $edit = FALSE, &$form_state) {
  if ($edit == FALSE) return;
  $value = array();

  foreach ($edit as $delta => $item) {
    $value[$delta]['currcode'] = $item['currcode'];

    if (!strlen($item['main_quant']) && empty($item['div_quant'])) {
      $value[$delta]['quantity'] = '';
    }
    elseif (is_numeric($item['main_quant']) || empty($item['main_quant'])) {
      $value[$delta]['quantity'] = floatval($item['main_quant']);
      if (!empty($item['div_quant'])) {
        $value[$delta]['quantity'] += ($item['div_quant'] + 0) / 100;
      }
    }
    elseif ($element['#calc']) {
      $value[$delta]['quantity'] = $item['main_quant'];
    }

    //set the value to zero if the field is empty and the currency allows it
    $currency = currency_load($item['currcode']);
    //we can remove the @ when everybody has upgraded to RC6
    if (empty($value[$delta]['quantity']) && @$currency->display['zero']) {
      $value[$delta]['quantity'] = '0';
    }
    elseif(empty($value[$delta]['quantity']) && empty($element['#preset'])) {
      unset($value[$delta]);
    }
  }
  //renumber the items or field_API will break!
  $value = array_values($value);
  //we have to set the form_state[values] here because they have probably already been set by the parent element
  //they are only set automatically if they haven't been set at all
  //drupal_array_set_nested_value($form_state['values'], $element['#array_parents'], @$value);
  return $value;
}


function _worth_element_process($element, &$form_state) {
  $element['#title_display'] = 'attribute';
  $main_quant = '';
  $quantity = @$element['#default_value']['quantity'];
  //the element is aware of a set of currcodes
  if (empty($element['#currcodes'])) {
    //best not rely on this, supply your own currencies that the user is permitted to use.
    //this defaults to all
    $element['#currcodes'] = array_keys(currencies_load($GLOBALS['user']));
  }
  $element['currcode'] = currency_picker_element($element['#currcodes'], @$element['#default_value']['currcode']);

  if (count($element['#currcodes']) == '1') {//and the currency chooser will be hidden
    $currency = currency_load(current($element['#currcodes']));
    $use_div_subelement = in_array($currency->display['divisions'], array(CURRENCY_DIVISION_MODE_CUSTOM, CURRENCY_DIVISION_MODE_CENTS_FIELD));
  }
  //work out the defaults for each subfield
  //null is not presumed to be zero, and is preserved
  if (!empty($use_div_subelement)) {
    list($main_quant, $div_quant) = currency_explode($quantity);
  }
  elseif($currency && $currency->display['divisions']) {
    $main_quant = $quantity;
  }
  else{
    $main_quant = intval($quantity);
  }
  //in normal mode, show a blank field where the default value is 0
  if (empty($main_quant) && empty($element['#preset'])) $main_quant = '';
  //while we return a float, the widget may want to show two fields.
  //in any case we show a main field, and an optional division field
  $element['main_quant'] = array(
    '#title' => t('Quantity'),//this is only really needed for errors, and maybe not even then
    '#title_display' => 'attribute',
    '#type' => 'textfield',
    '#default_value' => strlen($main_quant) ? $main_quant : '',
    '#size' => 4,
    '#maxlength' => 6,
    '#theme_wrappers' => array()//to prevent the default form_element wrapper
  );

  //we only show the division field if the currency requires it
  if (isset($div_quant)) {//that means $currency is also set

    if ($currency->display['divisions'] == CURRENCY_DIVISION_MODE_CUSTOM) {
      $element['div_quant'] = array(
        '#title' => t('Fraction'),
        '#title_display' => 'attribute',
        '#type' => 'select',
        '#options' => $currency->display['divisions_allowed'],
        '#default_value' => $div_quant
      );
    }
    else {//must be CURRENCY_DIVISION_MODE_CENTS_FIELD
      $element['div_quant'] = array(
        '#title' => t('Cents'),
        '#title_display' => 'attribute',
        '#type' => 'textfield',
        '#size' => 2,
        '#maxlength' => 2,
        '#default_value' => $div_quant,
        '#theme_wrappers' => array()//to prevent the default form_element wrapper
      );
    }
  }
  return $element;
}


/*
 * Default implementation of theme 'worth_field'
 * Converts the quantity into tokens and replaces them into the currency display format
 * $variables should have keys
 * 'quantity'
 * 'currcode'
 */
function theme_worth_item(&$variables) {
  $currency = currency_load($variables['currcode']);
  list($integer, $cents) = currency_explode($variables['quantity']);
  switch($currency->display['divisions']) {
    case CURRENCY_DIVISION_MODE_CUSTOM :
      $subdivision = @$currency->display['divisions_allowed'][intval($cents)];
      break;
    case CURRENCY_DIVISION_MODE_CENTS_FIELD:
    case CURRENCY_DIVISION_MODE_CENTS_INLINE:
      $subdivision = str_pad($cents, 2, STR_PAD_LEFT);
      break;
  }
  //I don't know how to account for different languages in number format
  $quantity = number_format(floatval($integer));
  if (isset($subdivision)) {
    $quantity .= $currency->display['delimiter'] . $subdivision;
  }
  $minus = $variables['quantity'] < 0 ? '-' : '';
  return $minus . str_replace('[quantity]', $quantity, $currency->display['format']);
}

/*
 * default theme implementation for callback 'worth_element'
 * called once per $item
 */
function theme_worth_element($variables) {
  $element = &$variables['element'];
  $element['#attributes']['class'][] = "worth-widget";
  $curr_select = drupal_render($element['currcode']);
  $quant_field = drupal_render($element['main_quant']);
  if ($element['currcode']['#type'] == 'hidden' || $element['currcode']['#theme'] == 'hidden') {//if the currency is preset, then we can use its own display format
    $currcode = isset($element['#value']) ? $element['#value']['currcode'] : $element['#default_value']['currcode'];
    $currency_display = currency_load($currcode)->display;
    if (isset($element['div_quant'])) {
      $quant_field .= $currency_display['delimiter'] . drupal_render($element['div_quant']);
    }
    $output = $curr_select . str_replace('[quantity]', $quant_field, $currency_display['format'], $vars);
  }
  else {//if there is no known currcode, the
    if (!empty($element['div_quant'])) {
      $quant_field .= drupal_render($element['div_quant']);
    }
    $output = $curr_select . $quant_field;
  }
  return '<div ' . drupal_attributes($element['#attributes']) . '>'. $output .'</div>';
}


function transaction_get_links($transaction, $ajax = TRUE, $view = FALSE) {
  if ($ajax) drupal_add_js('misc/ajax.js', array('type' => 'file'));
  global $language;
  static $operations;
  if (!$operations) {
    $operations = transaction_operations(TRUE, FALSE);
  }
  if (empty($transaction->serial)) {
    return array();//this happens on the transaction preview page
  }
  $renderable = array(
    '#theme' => 'links',
    //'#heading' => t('Operations'),
    '#attributes' => array('class' => array('transaction-operations')),
    '#links' => array()
  );
  $base = 'transaction/'. $transaction->serial;
  foreach ($operations as $op => $info) {
    if (!$view && $op == 'view') continue;
    if (transaction_operation_access($op, $transaction)) {
      $renderable['#links'][$op] = array(
        'title' => $info['title'],
        'href' =>  $base,
        'attributes' => $ajax ? array('class' => array('use-ajax')) : array()
      );
      if ($op != 'view') {
        $renderable['#links'][$op]['href'] .= '/nojs/'. $op;
      }
    }
  }
  return $renderable;
}

/*
 * Utility functions
 */

//returns an array of integer, hundredths, and minus sign if applicable
//if a the passed value is not a number, then pass it through
function currency_explode($number) {
  if (is_numeric($number)) {
    return array_merge(explode('.', number_format(abs($number), 2, '.', '')));
  }
  return array($number, 0);
}

/*
 * rounds a transaction quantity up or down, according to currency settings
 */
function mcapi_round($quant, $currcode, $up = FALSE) {
  $currency = currency_load($currcode);
  switch ($currency->display['divisions']) {
    case CURRENCY_DIVISION_MODE_CUSTOM:
      //calculate the number of hundredths from each division to the fraction, and choose the smallest
      list($int, $fraction) = array_pad(explode('.', $quant), 2, 0);
      foreach ($currency->display['divisions_allowed'] as $division) {
        $diffs[$division] = abs($fraction - $division);
      }
      asort($diffs);
      return "$int.". key($diffs);
      break;
    case CURRENCY_DIVISION_MODE_CENTS_FIELD:
    case CURRENCY_DIVISION_MODE_CENTS_INLINE:
      $quant = round($quant, 2);
    case CURRENCY_DIVISION_MODE_NONE:
      return $quant;
  }
}


function currency_picker_element(array $currcodes, $default = NULL, $multiple = 0) {
  $currencies = currencies_load();
  if (count($currcodes) > 1) {
    foreach ($currcodes as $currcode) {
      $options[$currcode] = $currencies[$currcode]->human_name;
    }
    return array(
      '#title' => t('Currency'),
      '#title_display' => 'attribute',
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default,
      '#multiple' => $multiple
    );
  }
  else {
    return array(
      '#type' => 'hidden',
      '#value' => current($currcodes)
    );
  }
}

/*
 * menu callback
 * build the most basic possible transaction form
 */
function transaction_default_form($form, &$form_state) {
  $form = transaction_form($form, $form_state);
  $form['state']['#type'] = 'value';
  $form['type']['#type'] = 'value';
  $form['creator']['#type'] = 'value';
  return $form;
}



//this is called via mcapi_dependent_transaction_action_form
//where we don't have the opportunity to form_load_include because it is called through the action system
function mcapi_configure_worth_accept_formula($element) {
  foreach (element_children($element) as $delta) {
    //this field will accept a formula, not just a number
    $element[$delta]['main_quant']['#size'] = 10;
    $element[$delta]['main_quant']['#maxlength'] = 10;
    $element[$delta]['main_quant']['#element_validate'] = array('validate_mcapi_configure_worth_formula');
  }
  return $element;
}
/*
 * element validate callback
 * ensures that the formula for calculating a quantity works
 */
function validate_mcapi_configure_worth_formula(&$element, $form_state) {
  if (empty($element['#value'])) return;
  if (!is_numeric(_mcapi_calc($element['#value'], 100))) {
    form_error($element, t('Invalid formula'));
  }
}
