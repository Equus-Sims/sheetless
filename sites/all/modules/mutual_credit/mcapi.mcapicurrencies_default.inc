<?php

/*
 * implements hook_mcapicurrencies_default
 * simply copies the currency in the default currency variable, used for single currency systems
 */
function mcapi_mcapicurrencies_default() {
  $currency = new stdClass;
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 3;
  $currency->currcode = 'credunit';
  $currency->data = (object)array(
    'info' => array(//this wierd structure is for ctools benefit
      'currcode' => $currency->currcode,
    ),
    'human_name' =>  t('Community Coins'),
    'issuance' => 'exchange',
    'display' => array(
      'divisions' => CURRENCY_DIVISION_MODE_CUSTOM,
      'divisions_setting' => "0|00 mins\n25|15 mins\n50|30 mins\n75|45 mins",
      'delimiter' => ':',
      'format' => '<strong>CC</strong>[quantity]',
      'zero' => '',
    ),
    'access' => array(
      'membership' => 'user_chooser_segment_perms:transact',
      'trader_data' => 'user_chooser_segment_perms:transact',
      'system_data' => 'user_chooser_segment_perms:transact',
    ),
    'view_transaction_states' => array(
      TRANSACTION_STATE_FINISHED => array(
        'transaction_access_callback_perm_transact' => 'transaction_access_callback_perm_transact',
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
      TRANSACTION_STATE_UNDONE => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
    ),
    'access_operations' => array(
      'undo' => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
      'sign' => array(
        'transaction_access_callback_pending_signatory' => 'transaction_access_callback_pending_signatory'
      ),
      'undo_pending' => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
      'signoff' => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      )
    ),
    'uid' => '1',
    'reservoir' => '1',
    'limits' => array(
      'limits_callback' => 'limits_global',
      'limits_global' => array(
        'min' => '0',
        'max' => '',
      ),
      'personal' => 0,
      'skip' => array(
        'reservoir' => 0,
        'auto' => 'auto',
        'mass' => 'mass',
        'user1' => 0,
        'owner' => 0,
      ),
    )
  );
  $currencies[$currency->currcode] = $currency;



  $mcapicurrency = new stdClass();
  $mcapicurrency->disabled = FALSE; /* Edit this to true to make a default mcapicurrency disabled initially */
  $mcapicurrency->api_version = 3;
  $mcapicurrency->currcode = 'escrow';
  $mcapicurrency->data = (object) array(
    'info' => array(
      'currcode' => $mcapicurrency->currcode,
    ),
    'human_name' => t('Euros'),
    'issuance' => 'commodity',
    'display' => array(
      'divisions' => CURRENCY_DIVISION_MODE_CENTS_INLINE,
      'divisions_setting' => '',
      'delimiter' => ':',
      'format' => '€[quantity]',
      'zero' => '',
    ),
    'access' => array(
      'membership' => 'user_chooser_segment_perms:transact',
      'trader_data' => 'user_chooser_segment_perms:transact',
      'system_data' => 'user_chooser_segment_perms:transact',
    ),
    'view_transaction_states' => array(
      TRANSACTION_STATE_FINISHED => array(
        'transaction_access_callback_perm_transact' => 'transaction_access_callback_perm_transact',
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
      TRANSACTION_STATE_UNDONE => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
    ),
    'access_operations' => array(
      'undo' => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
      'sign' => array(
        'transaction_access_callback_pending_signatory' => 'transaction_access_callback_pending_signatory'
      ),
      'undo_pending' => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      ),
      'signoff' => array(
        'transaction_access_callback_perm_manage_all' => 'transaction_access_callback_perm_manage_all'
      )
    ),
    'uid' => '1',
    'reservoir' => '1',
    'limits' => array(
      'limits_callback' => 'limits_global',
      'limits_global' => array(
        'min' => '0',
        'max' => '',
      ),
      'personal' => 1,
      'skip' => array(
        'reservoir' => 'reservoir',
        'auto' => 0,
        'mass' => 0,
        'user1' => 0,
        'owner' => 0,
      ),
    )
  );
  $currencies[$mcapicurrency->currcode] = $mcapicurrency;

  return $currencies;
}
