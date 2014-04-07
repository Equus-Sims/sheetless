<?php

/*
 * Implements variable module's hook_variable_info
 */
function mcapi_variable_info() {
  $vars['mcapi_mixed_transactions'] = array(
    'title' => t('Mixed transactions'),
    'description' => t('Whether to allow more than one transaction per currency'),
    'type' => 'boolean',
    'default' => TRUE,
    'group' => 'mcapi',
    'required' => FALSE,
  );
  $vars['transaction_description_field'] = array(
    'type' => 'select',
    'title' => t('Transaction description field'),
    'default' => '',
    'description' => t('Optional nominated fieldAPI field for describing the transaction'),
    'group' => 'mcapi',
    'options callback' => 'mcapi_variable_options_callback',
    'required' => FALSE,
  );
  $vars['mcapi_controller_delete_mode'] = array(
    'title' => t('Transaction delete mode'),
    'default' => 0,
    'description' => t('Whether transactions should be deleted, erased or offset'),
    'type' => 'select',
    'group' => 'mcapi',
    'options callback' => 'mcapi_variable_options_callback',
    'required' => FALSE,
  );
  $vars['mcapi_sentence'] = array(
    'type' => 'string',
    'title' => t('Transaction sentence template'),
    'default' => mcapi_sentence_default(),//this is already localised
    'description' => t('A tokenised string used for rendering a transaction in one sentence or statement.'),
    'group' => 'mcapi',
    'required' => TRUE,
  );
  $vars['mcapi_undo_notify'] = array(
    'type' => 'string',
    'title' => t('Transaction sentence template'),
    'default' => array(
      'subject' => '[site:name]: '. t('Your signature required'),
      'body' => '[transaction:url:absolute]'
    )
  );
  $vars['mcapi_show_balances'] = array(
    'type' => 'boolean',
    'title' => t('Show balances'),
    'description' => t('Whether to render the balances in the user profile (a block is already available)'),
    'group' => 'mcapi',
    'required' => FALSE,
    'default' => FALSE,
  );
  return $vars;
}

function __mcapi_variable_options_callback() {
  drupal_set_message('is this worth implementing?');
}

function __mcapi_variable_type_info() {
  //might want to make something for worth or worths
}

function mcapi_variable_group_info() {
  return array(
    'mcapi' => array(
      'title' => t('Accounting variables'),
      'description' => '',
      'access' => 'configure all currencies',
//      'path' => 'admin/config/regional/mcapi',
    )
  );
}
