<?php

/*
 * Implements ctools hook mcapi_default_forms
 * CTools loads this every time it wants to show a form - no caching very inefficient
 */
function bank_slave_mcapiforms_default() {
  mcapi_init();//ensures the currencies are available
  $forms = array();
  foreach (variable_get('mcapi_bank_slave_currcodes', array()) as $currcode) {
    foreach (variable_get('mcapi_bank_slave_uids', array()) as $bank_uid) {
      $name = 'bank_income_'.$currcode.'_'.$bank_uid;
      $mcapiform = new stdClass();
      $mcapiform->disabled = FALSE; /* Edit this to true to make a default mcapiform disabled initially */
      $mcapiform->api_version = 3;
      $mcapiform->name = $name;
      $mcapiform->status = 1;
      $mcapiform->data = array(
        'info' => array(
          'name' => $name,
        ),
        'architecture' => array(
          'path' => 'bank/'.$name,
          'menu_link' => 'navigation',
          'access' => 'user_chooser_segment_perms:log bank transactions',
          'use_block' => 1,
        ),
        'perspective' => '3',
        'payee' => array(
          'preset' => $bank_uid,
          'strip' => 1,
          'user_chooser_config' => 'user_chooser_segment_roles:2',
        ),
        'payer' => array(
          'preset' => currency_load('escrow')->reservoir,
          'strip' => 1,
          'user_chooser_config' => 'user_chooser_segment_roles:2',
        ),
        'worth' => array(
          'preset' => array(
            0 => array(
              'quantity' => 0,
              'currcode' => $currcode
            )
          ),
          'strip' => TRUE,
        ),
        'experience' => array(
          'title' => t(
            'Track !currcode income to !bank',
            array('!currcode' => currency_load($currcode)->human_name , '!bank' => user_load($bank_uid)->name)
          ),
          'template' => '[mcapiform:created]
            [mcapiform:worth]',
          'sure_title' => '',
          'sure_format' => 'certificate'
        ),
        'submission' => array(
          'button' => 'Submit',
          'redirect' => 'bank/'.$name,
          'feedback' => 'The transaction has been saved',
        ),
      );
      $forms[$mcapiform->name] = $mcapiform;

      $name = 'bank_outgoings_'.$currcode.'_'.$bank_uid;
      $mcapiform = new stdClass();
      $mcapiform->disabled = FALSE; /* Edit this to true to make a default mcapiform disabled initially */
      $mcapiform->api_version = 3;
      $mcapiform->name = $name;
      $mcapiform->status = 1;
      $mcapiform->data = array(
        'info' => array(
          'name' => $name,
        ),
        'architecture' => array(
          'path' => 'bank/'.$name,
          'menu_link' => 'navigation',
          'access' => 'user_chooser_segment_perms:log bank transactions',
          'use_block' => 1,
        ),
        'perspective' => '3',
        'payee' => array(
          'preset' => currency_load('escrow')->reservoir,
          'strip' => 1,
          'user_chooser_config' => 'user_chooser_segment_roles:2',
        ),
        'payer' => array(
          'preset' => $bank_uid,
          'strip' => 1,
          'user_chooser_config' => 'user_chooser_segment_roles:2',
        ),
        'worth' => array(
          'preset' => array(
            0 => array(
              'quantity' => 0,
              'currcode' => $currcode
            )
          ),
          'strip' => TRUE,
        ),
        'experience' => array(
          'title' => t(
            'Track !currcode outgoings from !bank',
            array('!currcode' => currency_load($currcode)->human_name , '!bank' => user_load($bank_uid)->name)
          ),
          'template' => '[mcapiform:created]
            [mcapiform:worth]',
          'sure_title' => '',
          'sure_format' => 'certificate'
        ),
        'submission' => array(
          'button' => 'Submit',
          'redirect' => 'bank/'.$name,
          'feedback' => 'The transaction has been saved',
        ),
      );
      $forms[$mcapiform->name] = $mcapiform;
    }
  }
  return $forms;
}