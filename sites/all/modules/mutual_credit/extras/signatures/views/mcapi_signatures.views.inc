<?php

/*
 * Implements views hook_views_data
 */
function mcapi_signatures_views_data() {

  $data['mcapi_signatures']['table']['group']  = t('Signatures');
  $data['mcapi_signatures']['table']['base'] = array(
    'field' => 'serial',
    'title' => t('Signatures'),
    'help' => t('Signatures needed on pending transactions'),
    'access query tag' => 'transaction_access_views',
    'defaults' => array(
      'field' => 'serial',
    ),
  );
  $data['mcapi_signatures']['table']['join'] = array(
    'mcapi_transactions' => array(
      'left_field' => 'serial',
      'field' => 'serial',
      'type' => 'LEFT',
    ),
  );

  //some of these are a bit confused - need more time!
  $data['mcapi_signatures']['uid'] = array(
    'title' => t('Signatory'),
    'description' => t("The user's signature on a transaction"),
    'field' => array(
      'help' => t('Signatory to a transaction'),
      'handler' => 'views_handler_field_user'
    ),
    'filter' => array(
      'title' => t('Current user is a signatory'),
      'help' => t('Show only transactions on which the current user is a signatory'),
      'handler' => 'views_handler_filter_user_current'
    ),
    'argument' => array(
      'title' => t('Passed user is a signatory'),
      'handler' => 'views_handler_argument_user_uid',
    ),
    //for some reason this isn't showing a widget so that fields can connect to it
    'relationship' => array(
      'label' => t('Signatory user'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
    ),
  );
  $data['mcapi_signatures']['pending'] = array(
    'title' => t('Signature needed'),
    'help' => t('Whether the signature is still needed'),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_filter_sort',
    )
  );
  return $data;
}


function mcapi_signatures_views_data_alter(&$data) {
  // For other base tables, explain how we join
  $data['mcapi_transactions']['table']['join']['mcapi_signatures'] = array(
    'left_table' => 'mcapi_signatures',
    'left_field' => 'serial',
    'field' => 'serial',
  );
}
