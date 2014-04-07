<?php

/*
 * In this file we build the form, using the object cached in ctools
 */
module_load_include('inc', 'mcapi');

/*
 * main form builder, for this module, using the transaction_form as a base
 * build_info args are 0 => transaction, 1 => mcapi_form
 * $form['#settings'] is already defined
 * would have been nice to be able to build the transaction from the settings and pass it to transaction_form
 * then not worry about setting defaults here, which is really about shaping the form.
 */
function mcapi_form_build(&$form, &$form_state, $form_id) {
  $form['#settings'] =  $form_state['build_info']['args'][MCAPIFORM_BUILD_INFO_ARG];
  $settings = &$form['#settings'];
  $transaction = &$form['#transaction'];
  //change the 5 translatable strings
  if (function_exists('i18n_string_object_translate')) {
    //prepare the i18n pseudo_object, because i18n expects a flat object and the mcapi_form just isn't, sorry
    $object = _mcapi_i18n_object($settings);
    //translate the object
    $translations = i18n_string_object_translate('payform_experience', $object);
    //and copy the strings back into the settings
    $settings['experience']['title'] = $translations['title'];
    $settings['experience']['template'] = $translations['template'];
    $settings['experience']['sure_title'] = $translations['sure_title'];
    $settings['submission']['button'] = $translations['button'];
    $settings['submission']['feedback'] = $translations['feedback'];
    $settings['direction']['incoming'] = $translations['incoming'];
    $settings['direction']['outgoing'] = $translations['outgoing'];
  }

  if (!empty($form_state['storage']) && $form_state['step'] == 1) {
    $form_state['values'] = $form_state['storage'];
  }
  if (empty($form_state['step'])) {
    if ($settings['experience']['title']) {
      $form_state['storage'] = array();
      $form_state['step'] = 1;
    }
    else {//jump to step 2 if there's no title for step 1
      $form_state['step'] = 2;
    }
  }
  //set the page title
  if ($form_state['step'] == 1) {
    drupal_set_title(filter_xss($settings['experience']['title']), PASS_THROUGH);
  }
  else {
    drupal_set_title(filter_xss($settings['experience']['sure_title']), PASS_THROUGH);
  }

  //we need to store the step value in $form so that the theme system knows what to do
  $form['#step'] = $form_state['step'];
  $form['#theme'] = 'mcapi_form';
  //this might not be the way to build a 2 step form.
  //All the validation callbacks need to check the step because all the important fields are in step one only.
  if ($form['#step'] == 1) {
    $form['#action'] = url($settings['architecture']['path']);
    $form['#template'] = $settings['experience']['template'];
    $form['creator']['#type'] = 'hidden';
    $form['state']['#type'] = 'hidden';
    $form['state']['#value'] = TRANSACTION_STATE_FINISHED;
    $form['type']['#type'] = 'value';
    $form['type']['#value'] = $settings['info']['name'];
     //participants
    if ($settings['perspective'] == 3) {
      foreach (array('payer', 'payee') as $participant) {
        //specifies which user_chooser widget
        $uchoo_setup = explode(':', $settings[$participant]['user_chooser_config']);
        $form[$participant]['#type'] = 'user_chooser_few';
        $form[$participant]['#callback'] = array_shift($uchoo_setup);
        $form[$participant]['#args'] = $uchoo_setup;
        $form[$participant]['#multiple'] = FALSE;
      }
    }
    else {
      //I-thou exchanges
      $form['firstperson'] = array(
        '#type' => 'hidden',
        '#value' => $GLOBALS['user']->uid
      );
      $form['secondperson'] = array(
        '#title' => t('Other person'), //this isn't supposed to be rendered
      ) + $form['payee'];
      if (property_exists($transaction, 'secondperson')) {
        $form['secondperson']['#default_value'] = $transaction->secondperson;
      }
      //specifies which user_chooser widget
      $uchoo_setup = explode(':', $settings['secondperson']['user_chooser_config']);
      $form['secondperson']['#type'] = 'user_chooser_few';
      $form['secondperson']['#callback'] = array_shift($uchoo_setup);
      $form['secondperson']['#args'] = $uchoo_setup;
      $form['secondperson']['#required'] = TRUE;
      $form['secondperson']['#multiple'] = FALSE;
      $form['secondperson']['#exclude'] = array($GLOBALS['user']->uid);
      $form['secondperson']['#default_value'] = $settings['secondperson']['preset'];
      $type = $settings['direction']['widget'] ? $settings['direction']['widget'] : 'select';
      $options = ($type == 'select' && empty($settings['direction']['preset'])) ? array(t('Please choose...')) : array();
      $form['direction'] = array(
        '#title' => t('Direction'),
        '#type' => $type,
        '#options' => $options + array(
          'incoming' => $settings['direction']['incoming'],
          'outgoing' => $settings['direction']['outgoing']
        ),
        //if we are loading the page from blank, or rebuilding the form, we set the default values here,
        '#default_value' =>  property_exists($transaction, 'direction') ? strval($transaction->direction) : '',
        '#required' => TRUE
      );
      unset($form['payer'], $form['payee']);
      //this validation function must run BEFORE the main one in the API, because its add payer and payee, which are required for normal validation
      array_unshift($form['#validate'], 'mcapi_forms_validate_secondperson');
    }
    if (strpos($settings['experience']['template'], 'created]')) {
      if (module_exists('date_popup')) {
        $form['created'] = array(
          '#type' => 'date_popup',//this doens't work
          '#date_type' => DATE_UNIX
        );
      }
      //else {
        $form['created'] = array(
          '#type' => 'date',
          '#element_validate' => array('mcapi_date_validate')
        );
      //}
    }
    if ($form['#settings']['experience']['sure_title']) {
      $form['buttons']['submit']['#value'] = t('Preview');
    }
  }
  elseif ($form['#step'] == 2) {
    //we unset most of the fields so they don't display or change from what's in storage
    foreach(element_children($form) as $fieldname) {
      if (in_array($fieldname, array('buttons', 'form_id', 'form_build_id', 'form_token'))) continue;
      unset($form[$fieldname]);
    }
    //put the back button only for last step if there are values from step 1
    if (!empty($form_state['values'])) {//coming from step 1
      $transactions = &$form_state['transactions'];//these have no xid keys
      mcapi_hide_tabs();
      $form['buttons']['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        '#weight' => -1
      );
      array_unshift($form['#submit'], 'mcapi_forms_form_presubmit');
    }
    else {//landing directly on step 2
      $transaction = &$form_state['build_info']['args'][0];
      $transactions[$transaction->xid] = $transaction;
    }
    //for the preview, we need to put any dependent transactions inside the main one
    while(count($transactions) > 1) {
      $transactions[0]->dependents[] = array_pop($transactions);
    }
    //the tokens page already has a token selector for the main form, based on mcapiform
    //rather than make another token selector just for the rare use-case of using custom tokens,
    //the sure_format is made with [mcapiform] tokentype and here replaced for [transaction]
    $view_mode = $settings['experience']['sure_format'] == 'token' ?
      str_replace('[mcapiform:', '[transaction:', $settings['experience']['sure_template']) :
      $settings['experience']['sure_format'];
    //then the view_mode, if unknown, is interpreted as a token
    $form['#preview'] = transaction_view($transactions, $view_mode);
    //the transaction properties were validated in step 1
    //bypass the default transaction validate function, transaction_form_validate which expects stuff in form_state['values]
    unset($form['#validate']);
    //save button
    $form['buttons']['#type'] = 'actions';
    if (strpos($settings['submission']['button'], '/')) {
      $form['buttons']['submit']['#type'] = 'image_button';
      $form['buttons']['submit']['#src'] = $settings['submission']['button'];
    }
    else {
      $form['buttons']['submit']['#value'] = $settings['submission']['button'];
    }
    if (form_get_errors()) {
      unset($form['buttons']['submit']);
    }
  }
  //if we just put the strip at the top level of each element,
  //then it will get lost as the element-language-items go to theming
  foreach (element_children($form) as $fieldname) {
    if (isset($settings[$fieldname]['strip'])) {
      //not a very rigourous way to determine
      if(isset($form[$fieldname]['#title'])) {//normal fields
        $form[$fieldname]['#strip'] = $settings[$fieldname]['strip'];
      }
      else {//fieldAPI
        //I think we can get away with assuming that all transactions and
        //all fieldAPI fields on them are LANGUAGE_NONE
        $form[$fieldname][LANGUAGE_NONE]['#strip'] = $settings[$fieldname]['strip'];
      }
    }
  }
  array_push($form['#submit'], 'mcapi_forms_form_postsubmit');//can't remember why use array_push and not just [] operator
  $form['#validate'][] = 'mcapi_forms_validate';//this goes last
  $form['#attributes']['class'][] = 'step-'.$form['#step'];
  $form['#contextual_links']['mcapi_forms'] = array('admin/accounting/forms/list', array($settings['info']['name']));
}


/*
 * $variables consists of the usual suspects plus
 * $template
 * $mcapi_form_data
 */
function theme_mcapi_form($variables) {
  $form = &$variables['form'];
  $form['buttons']['#strip'] = TRUE;
  //render the visible stuff first
  if ($form['#step'] == 1) {
    foreach (element_children($form) as $fieldname) {
      $properties = array('payer', 'payee', 'secondperson', 'direction', 'actions');
      if (in_array($fieldname, $properties)) {
        if (isset($form[$fieldname]['#strip']) && $form[$fieldname]['#strip'] || $fieldname == 'buttons') {
          unset($form[$fieldname]['#theme_wrappers']);
          unset($form[$fieldname]['#title']);
          unset($form[$fieldname]['#description']);
        }
      }
      else {
        //strip the field API fields of their form-item wrappers
        if (!empty($form[$fieldname][LANGUAGE_NONE]['#strip'])) {
          //but they have different structures for radios, checkboxes and worths
          if (isset($form[$fieldname][LANGUAGE_NONE]['#entity'])) {
            unset($form[$fieldname][LANGUAGE_NONE]['#theme_wrappers']);
            //for some reason, getting rid of the theme wrappers on radio buttons doesn't work
            //but removing title and description is mostly equivalent
            unset($form[$fieldname][LANGUAGE_NONE]['#title']);
            unset($form[$fieldname][LANGUAGE_NONE]['#description']);
          }
          //compared to normal fields
          elseif(isset($form[$fieldname][LANGUAGE_NONE][0]['#entity']) || isset($form[$fieldname][LANGUAGE_NONE][0]['tid']['#entity'])) {
            mcapi_unset_recursive($form[$fieldname], '#theme_wrappers');
          }
        }
      }
    }
    return token_replace(
      check_markup($form['#template'], MCAPIFORM_STRING_FORMAT),
      array('mcapiform' => $form, 'user' => $GLOBALS['user']),
      array('sanitize' => FALSE)
    );
  }
  elseif ($form['#step'] == 2) {
    $preview = drupal_render($form['#preview']);
    return $preview . drupal_render_children($form) . drupal_render($form['buttons']);
  }
}

/*
 * Form validation callback
 * set the payer and payee from the 2ndpeson and direction
 */
function mcapi_forms_validate_secondperson($form, &$form_state) {
  $values = &$form_state['values'];
  if (empty($values['secondperson'])) {//this is a rare situation, in which the user_chooser removed the value
    form_set_error('secondperson', t('Who was it with?'));
  }
  if ($values['direction'] == 'incoming') {
    $values['payer'] = $values['secondperson'];
    $values['payee'] = $values['firstperson'];
  }
  elseif ($values['direction'] == 'outgoing') {
    $values['payer'] = $values['firstperson'];
    $values['payee'] = $values['secondperson'];
  }
}

/*
 * form validation callback
 * handles the steps
 */
function mcapi_forms_validate($form, &$form_state) {
  if (!empty($form_state['clicked_button']['#submit']) && in_array('mcapi_transaction_undo_confirm_form', $form_state['clicked_button']['#submit'])) {
    return;
  }

  $form_state['rebuild'] = FALSE;
  //go back if that button was pressed
  if (isset($form_state['values']['back']) && $form_state['values']['op'] == $form_state['values']['back']) {
    $form_state['rebuild'] = TRUE;
    $form_state['step']--;
    $form_state['values'] = $form_state['storage'];
    unset($form_state['transactions']);
  }
  //if step 2 template is available add a step and rebuild the form
  elseif ($form['#settings']['experience']['sure_title']) {
    if (!form_get_errors() && $form['#step'] == 1) {
      $form_state['storage'] = $form_state['values'] + $form_state['storage'];
      $form_state['rebuild'] = TRUE;
      $form_state['step']++;
    }
  }
}

/*
 * First submit callback for default transaction form
 * copies anything in $form_state[storage] back to form_state[values]
 * the $form_state['values'] is step 2 were only needed to render the tokens
 * the actual transaction values were in $form_state['storage']
 */
function mcapi_forms_form_presubmit($form, &$form_state) {
  $form_state['values'] = $form_state['storage'];
}

/*
 * Last submit callback for default transaction form
 * this has to be after the transaction id is created and before the end of form processing
 */
function mcapi_forms_form_postsubmit($form, &$form_state) {
  $submission = &$form['#settings']['submission'];
  if (isset($submission['feedback']) && $submission['feedback']) {
    //this could be more sophisticated by using tokens and transaction_load_last()
    drupal_set_message($submission['feedback']);
  }
  $serial = &$form_state['values']['serial'];
  //$form_state['redirect'] doesn't work here
  if (empty($submission['redirect'])) {
    drupal_goto('transaction/' . $serial);
  }
  else {
    $replacements = array('%user' => $GLOBALS['user']->uid, '%transaction' => $serial);
    drupal_goto(strtr($submission['redirect'], $replacements));
  }
}


function mcapi_unset_recursive(array &$array, $prop) {
  unset($array[$prop]);
  foreach($array as $key => &$val) {
    if (is_array($val)) {
      mcapi_unset_recursive($val, $prop);
    }
  }
}

/*
 * pre php5.3
 */
if (!function_exists('array_replace_recursive')) {
  function array_replace_recursive($array, $array1) {
    // handle the arguments, merge one by one
    $args = func_get_args();
    $array = $args[0];
    if (!is_array($array)) {
      return $array;
    }
    for ($i = 1; $i < count($args); $i++) {
      if (is_array($args[$i])) {
        $array = recurse($array, $args[$i]);
      }
    }
    return $array;
  }
  function recurse($array, $array1) {
    foreach ($array1 as $key => $value) {
      // create new key in $array, if it is empty or not an array
      if (!isset($array[$key]) || (isset($array[$key]) && !is_array($array[$key]))) {
        $array[$key] = array();
      }
      // overwrite the value in the base array
      if (is_array($value)) {
        $value = recurse($array[$key], $value);
      }
      $array[$key] = $value;
    }
    return $array;
  }
}

function mcapi_date_validate(&$element, &$form_state) {
  if (!array_key_exists('created', $form_state['values']));
  date_validate($element, $form_state);
  $d = $form_state['values']['created'];
  form_set_value($element, strtotime($d['year'] .'-'. $d['month'] .'-'. $d['day']), $form_state);
}
