<?php


/*
 * implements hook_variable_info
 * note this is from the variable modulek which may or may not be installed
 */
function bank_slave_variable_info() {
  return array(
    'mcapi_bank_slave_uid' => array(
      'title' => t('Account which holds subscriptions'),
      'description' => t('A username, email, or user ID'),
    ),
  );
}
