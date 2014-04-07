<?php
//$Id:

/*
 * implements hook_views_data_alter
 */
function mcapi_limits_views_data_alter(&$data) {
  $data['users']['limits'] = array(
    'title' => t('Balance limits'),
    'field' => array(
      'help' => t("The trading limits for a user in a currency or currencies"),
      'handler' => 'views_handler_field_mcapi_limits',
    )
  );
}