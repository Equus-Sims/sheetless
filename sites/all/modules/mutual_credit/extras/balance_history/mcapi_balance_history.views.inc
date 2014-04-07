<?php
// $Id:

function mcapi_balance_history_views_data_alter(&$data) {
  $data['mcapi_index']['balance_history'] = array(
    'title' => t('Balance_history'),
    'help' => t("Chart"),
    'field' => array(
      'handler' => 'views_handler_field_mcapi_balance_history',
      'additional fields' => array('uid1')
    )
  );
}
