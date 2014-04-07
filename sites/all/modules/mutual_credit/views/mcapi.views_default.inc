<?php
//Id;

function mcapi_views_default_views() {
  $view = new view;
  $view->name = 'transactions';
  $view->description = '';
  $view->tag = 'mcapi';
  $view->base_table = 'mcapi_transactions';
  $view->human_name = 'Transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = t('Transactions');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'name_1' => 'name_1',
    'worth' => 'worth',
    'view' => 'view',
  );
  /* Relationship: Transaction: Payee */
  $handler->display->display_options['relationships']['payee']['id'] = 'payee';
  $handler->display->display_options['relationships']['payee']['table'] = 'mcapi_transactions';
  $handler->display->display_options['relationships']['payee']['field'] = 'payee';
  $handler->display->display_options['relationships']['payee']['required'] = 0;
  /* Relationship: Transaction: Payer */
  $handler->display->display_options['relationships']['payer']['id'] = 'payer';
  $handler->display->display_options['relationships']['payer']['table'] = 'mcapi_transactions';
  $handler->display->display_options['relationships']['payer']['field'] = 'payer';
  $handler->display->display_options['relationships']['payer']['required'] = 0;
  /* Field: Transaction: Serial number */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'mcapi_transactions';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['exclude'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'payee';
  $handler->display->display_options['fields']['name']['label'] = 'Payee';
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'payer';
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name_1']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name_1']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name_1']['format_username'] = 1;
  /* Field: Transaction: Worth */
  $handler->display->display_options['fields']['worth']['id'] = 'worth';
  $handler->display->display_options['fields']['worth']['table'] = 'field_data_worth';
  $handler->display->display_options['fields']['worth']['field'] = 'worth';
  $handler->display->display_options['fields']['worth']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['worth']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['worth']['hide_empty'] = 0;
  $handler->display->display_options['fields']['worth']['empty_zero'] = 0;
  $handler->display->display_options['fields']['worth']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['worth']['click_sort_column'] = 'currcode';
  $handler->display->display_options['fields']['worth']['type'] = 'worth_field';
  $handler->display->display_options['fields']['worth']['field_api_classes'] = 0;

  if ($fieldname = variable_get('transaction_description_field')) {
    /* Field: Transaction: Description */
    $handler->display->display_options['fields'][$fieldname]['id'] = $fieldname;
    $handler->display->display_options['fields'][$fieldname]['table'] = 'field_data_'.$fieldname;
    $handler->display->display_options['fields'][$fieldname]['field'] = $fieldname;
  }
  /* Field: Transaction: Operations */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'mcapi_transactions';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['link_to_transaction'] = 0;

  /* Sort criterion: Transaction: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'mcapi_transactions';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Transaction: State */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'mcapi_transactions';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(1 => 1);//1 means finished transactions
  /* Filter criterion: Transaction: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'mcapi_transactions';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'not in';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dependent' => 'dependent',
    'mass' => 'mass',
  );
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['required'] = FALSE;
  $handler->display->display_options['filters']['type']['expose']['remember'] = FALSE;
  $handler->display->display_options['filters']['type']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Transactions', 'transactions');
  $handler->display->display_options['path'] = 'transactions';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Transactions';
  $handler->display->display_options['menu']['name'] = 'navigation';

  if ($fieldname = variable_get('transaction_description_field')) {
    /* Display: Who gave what */
    $handler = $view->new_display('block', 'Who gave what', 'who_gave_what');
    $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
    $handler->display->display_options['defaults']['pager'] = FALSE;
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '5';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['style_plugin'] = FALSE;
    $handler->display->display_options['style_plugin'] = 'list';
    $handler->display->display_options['style_options']['default_row_class'] = FALSE;
    $handler->display->display_options['style_options']['row_class_special'] = FALSE;
    $handler->display->display_options['defaults']['style_options'] = FALSE;
    $handler->display->display_options['defaults']['row_plugin'] = FALSE;
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['row_options']['inline'] = array(
      'name' => 'name',
      $fieldname => $fieldname,
    );
    $handler->display->display_options['defaults']['row_options'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: User: Nom */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'payee';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    /* Field: Transaction: Description */
    $handler->display->display_options['fields'][$fieldname]['id'] = 'transaction_description';
    $handler->display->display_options['fields'][$fieldname]['table'] = 'field_data_'.$fieldname;
    $handler->display->display_options['fields'][$fieldname]['field'] = $fieldname;
    $handler->display->display_options['fields'][$fieldname]['label'] = '';
    $handler->display->display_options['fields'][$fieldname]['alter']['alter_text'] = TRUE;
    $handler->display->display_options['fields'][$fieldname]['alter']['text'] = "[name] gave [$fieldname]";
    $handler->display->display_options['fields'][$fieldname]['element_label_colon'] = FALSE;
  }
  $views[$view->name] = $view;

  return $views;
}

$translatables['transactions'] = array(
  t('Payee'),
  t('Payer'),
  t('Serial number'),
  t('Name'),
  t('Worth'),
  t('Value'),
  t('Who paid whom'),
  t('[name] paid [name_1]'),
  t('Income & Expenditure'),
  t('Transactions'),
  t('Trade'),
  t("This year's transactions"),
);
