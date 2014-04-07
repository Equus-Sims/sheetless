<?php
currencies_load();

/*
 * Implements ctools hook mcapi_default_forms
 * N.B. Because ctools doesn't seem to run cache as well as might be expected, this is called any time a form is needed
 * Also: these are saved in the menu item arguments which may cause unexpected cacheing behaviour
 */
function mcapi_forms_mcapiforms_default() {
  $forms = array('1stparty', '3rdparty');
  foreach ($forms as $form_id) {
    $function = 'mcapiform_default_'.$form_id;
    $mcapiforms[$form_id] = $function();
    if (function_exists('i18n_string_object_update')) {
      $strings = array('name' => $form_id) + $mcapiforms[$form_id]->data['experience'];
      //i18n_string_object_update('payform_experience', $strings);
    }
  }
  return $mcapiforms;
}

function mcapiform_default_1stparty() {
  $currencies = currencies_load();
  $mcapiform = new stdClass;
  $mcapiform->disabled = FALSE; /* Edit this to true to make a default mcapi_form disabled initially */
  $mcapiform->api_version = 3;
  $mcapiform->name = '1stparty';
  $mcapiform->data = array(
    'info' => array(
      'name' => '1stparty',
    ),
    'perspective' => '1',
    'architecture' => array(
      'path' => 'transact/1stparty',
      'access' => 'user_chooser_segment_perms:transact',
      'menu_link' => MENU_DEFAULT_LOCAL_TASK,
      'use_block' => 1,
    ),
    'secondperson' => array(
      'preset' => '',
      'user_chooser_config' => 'user_chooser_segment_perms:transact',
      'strip' => 1,
    ),
    'direction' => array(
      'preset' => 'outgoing',
      'incoming' => 'Request',
      'outgoing' => 'Payment',
      'widget' => 'select',
      'strip' => 1,
    ),
    'worth' => array(
      'preset' => array(),
      'strip' => 1,
    ),
    'submission' => array(
      'button' => 'Submit',
      'redirect' => '',
      'feedback' => ''
    ),
    'experience' => array(
      'title' => 'Make a payment',
      'template' => 'Partner: [mcapiform:secondperson]
Direction: [mcapiform:direction]
[mcapiform:worth]',
      'sure_title' => 'Are you sure?',
      'sure_format' => 'certificate',
    )
  );
  return $mcapiform;
}

function mcapiform_default_3rdparty() {
  $currencies = currencies_load();
  $mcapiform = new stdClass;
  $mcapiform->disabled = FALSE; /* Edit this to true to make a default form disabled initially */
  $mcapiform->api_version = 3;
  $mcapiform->name = '3rdparty';
  $mcapiform->data = array(
    'info' => array(
      'name' => '3rdparty',
    ),
    'perspective' => '3',
    'architecture' => array(
      'path' => 'transact/3rdparty',
      'access' => 'user_chooser_segment_perms:manage all transactions',
      'menu_link' => MENU_LOCAL_TASK,
      'use_block' => 1,
    ),
    'payer' => array(
      'preset' => '',
      'user_chooser_config' => 'user_chooser_segment_perms:transact',
      'strip' => 1,
    ),
    'payee' => array(
      'preset' => '',
      'user_chooser_config' => 'user_chooser_segment_perms:transact',
      'strip' => 1,
    ),
    'worth' => array(
      'preset' => array(),
      'strip' => 1,
    ),
    'submission' => array(
      'button' => 'Submit',
      'redirect' => '',
      'feedback' => ''
    ),
    'experience' => array(
      'title' => 'Record a transaction',
      'template' => 'Payer: [mcapiform:payer]
Payee: [mcapiform:payee]
[mcapiform:worth]',
      'sure_title' => 'Are you sure?',
      'sure_format' => 'certificate',
    ),
  );
  return $mcapiform;
}

/*
 * implements hook_mcapiforms_default_alter
 * ensure that the transaction _description field is at least present on all default forms
 */
function mcapi_forms_mcapiforms_default_alter($forms){
  if ($fieldname = variable_get('transaction_description_field')) {
    foreach ($forms as $fid => $mcapiform){
      if (isset($forms[$fid]->data[$fieldname])) continue;
      $forms[$fid]->data[$fieldname] = array(
        'strip' => 1,
        'preset' => array(
          'und' => array(
            0 => array(
              'value' => '',
            ),
          ),
        ),
      );
      $forms[$fid]->data['experience']['template'] .= "\n".t('Description:') .' '. "[mcapiform:$fieldname]";
    }
  }
}

