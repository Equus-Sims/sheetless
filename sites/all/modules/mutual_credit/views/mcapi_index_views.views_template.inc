<?php


/*
 * implements hook_views_templates
 */
function mcapi_index_views_views_templates() {
  currencies_load(); //because this seems to run before mcapi_init()
  $currencies = currencies_load();
  $multicurrencies = count($currencies) > 1;

  if (module_exists('date_views')) {
    $view = new view();
    $view->name = 'income_expenditure';
    $view->description = 'two displays showing a users transactions in each direction, by period';
    $view->tag = 'mcapi';
    $view->base_table = 'mcapi_transactions';
    $view->human_name = 'Income & Expenditure';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'none';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'date_views_pager';
    $handler->display->display_options['pager']['options']['pager_position'] = 'bottom';
    $handler->display->display_options['style_plugin'] = 'table';
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['area']['id'] = 'area';
    $handler->display->display_options['empty']['area']['table'] = 'views';
    $handler->display->display_options['empty']['area']['field'] = 'area';
    $handler->display->display_options['empty']['area']['label'] = 'empty text';
    $handler->display->display_options['empty']['area']['empty'] = TRUE;
    $handler->display->display_options['empty']['area']['content'] = 'No transactions';
    $handler->display->display_options['empty']['area']['format'] = 'plain_text';
    /* Relationship: Transaction: Payee */
    $handler->display->display_options['relationships']['payee']['id'] = 'payee';
    $handler->display->display_options['relationships']['payee']['table'] = 'mcapi_transactions';
    $handler->display->display_options['relationships']['payee']['field'] = 'payee';
    $handler->display->display_options['relationships']['payee']['required'] = TRUE;
    /* Relationship: Transaction: Payer */
    $handler->display->display_options['relationships']['payer']['id'] = 'payer';
    $handler->display->display_options['relationships']['payer']['table'] = 'mcapi_transactions';
    $handler->display->display_options['relationships']['payer']['field'] = 'payer';
    $handler->display->display_options['relationships']['payer']['required'] = TRUE;
    /* Field: Transaction: Serial number */
    $handler->display->display_options['fields']['serial']['id'] = 'serial';
    $handler->display->display_options['fields']['serial']['table'] = 'mcapi_transactions';
    $handler->display->display_options['fields']['serial']['field'] = 'serial';
    /* Sort criterion: Transaction: created */
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'mcapi_transactions';
    $handler->display->display_options['sorts']['created']['field'] = 'created';

    /* Display: Income */
    $handler = $view->new_display('page', 'Income', 'page');
    $handler->display->display_options['defaults']['fields'] = FALSE;
    $handler->display->display_options['defaults']['header'] = FALSE;
    /* Header: Global: Text area */
    $handler->display->display_options['header']['area']['id'] = 'area';
    $handler->display->display_options['header']['area']['table'] = 'views';
    $handler->display->display_options['header']['area']['field'] = 'area';
    $handler->display->display_options['header']['area']['empty'] = TRUE;
    $handler->display->display_options['header']['area']['content'] = '<h4>'.t('Income').'</h4>';
    $handler->display->display_options['header']['area']['format'] = 'plain_text';

    /* Field: Transaction: Serial number */
    $handler->display->display_options['fields']['serial']['id'] = 'serial';
    $handler->display->display_options['fields']['serial']['table'] = 'mcapi_transactions';
    $handler->display->display_options['fields']['serial']['field'] = 'serial';
    $handler->display->display_options['fields']['serial']['link_to_transaction'] = 0;
    /* Field: Transaction: created */
    $handler->display->display_options['fields']['created']['id'] = 'created';
    $handler->display->display_options['fields']['created']['table'] = 'mcapi_transactions';
    $handler->display->display_options['fields']['created']['field'] = 'created';
    $handler->display->display_options['fields']['created']['label'] = '';
    $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['created']['date_format'] = 'short';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'payer';
    /* Field: Transaction: Worth */
    $handler->display->display_options['fields']['worth']['id'] = 'worth';
    $handler->display->display_options['fields']['worth']['table'] = 'field_data_worth';
    $handler->display->display_options['fields']['worth']['field'] = 'worth';
    $handler->display->display_options['fields']['worth']['label'] = 'Value';
    $handler->display->display_options['fields']['worth']['click_sort_column'] = 'currcode';
    $handler->display->display_options['fields']['worth']['type'] = 'worth_field';
    /* Field: Transaction: Description */
    if ($fieldname = variable_get('transaction_description_field')) {
      $handler->display->display_options['fields'][$fieldname]['id'] = $fieldname;
      $handler->display->display_options['fields'][$fieldname]['table'] = 'field_data_'.$fieldname;
      $handler->display->display_options['fields'][$fieldname]['field'] = $fieldname;
      $handler->display->display_options['defaults']['arguments'] = FALSE;
    }

    $handler->display->display_options['defaults']['arguments'] = FALSE;
    /* Contextual filter: Transaction: Payee */
    $handler->display->display_options['arguments']['payee']['id'] = 'payee';
    $handler->display->display_options['arguments']['payee']['table'] = 'mcapi_transactions';
    $handler->display->display_options['arguments']['payee']['field'] = 'payee';
    $handler->display->display_options['arguments']['payee']['default_action'] = 'default';
    $handler->display->display_options['arguments']['payee']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['payee']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['payee']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['payee']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['payee']['summary_options']['items_per_page'] = '25';
    /* Contextual filter: Date: Date (mcapi_transactions) */
    $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
    $handler->display->display_options['arguments']['date_argument']['table'] = 'mcapi_transactions';
    $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
    $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
    $handler->display->display_options['arguments']['date_argument']['year_range'] = '-10:+0';
    $handler->display->display_options['arguments']['date_argument']['granularity'] = 'month';
    $handler->display->display_options['arguments']['date_argument']['use_fromto'] = 'no';
    $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
      'mcapi_transactions.created' => 'mcapi_transactions.created',
    );
    $handler->display->display_options['path'] = 'user/%/income_expenditure';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Trade';
    $handler->display->display_options['menu']['description'] = 'This year\'s transactions';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['context'] = 0;


    /* Display: Expenditure */
    $handler = $view->new_display('attachment', 'Expenditure', 'attachment_1');
    $handler->display->display_options['defaults']['header'] = FALSE;
    /* Header: Global: Text area */
    $handler->display->display_options['header']['area']['id'] = 'area';
    $handler->display->display_options['header']['area']['table'] = 'views';
    $handler->display->display_options['header']['area']['field'] = 'area';
    $handler->display->display_options['header']['area']['empty'] = TRUE;
    $handler->display->display_options['header']['area']['content'] = '<h4>'.t('Expenditure').'</h4>';
    $handler->display->display_options['header']['area']['format'] = 'plain_text';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: Transaction: Serial number */
    $handler->display->display_options['fields']['serial']['id'] = 'serial';
    $handler->display->display_options['fields']['serial']['table'] = 'mcapi_transactions';
    $handler->display->display_options['fields']['serial']['field'] = 'serial';
    /* Field: Transaction: created */
    $handler->display->display_options['fields']['created']['id'] = 'created';
    $handler->display->display_options['fields']['created']['table'] = 'mcapi_transactions';
    $handler->display->display_options['fields']['created']['field'] = 'created';
    $handler->display->display_options['fields']['created']['label'] = '';
    $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['created']['date_format'] = 'short';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'payee';
    /* Field: Transaction: Worth */
    $handler->display->display_options['fields']['worth']['id'] = 'worth';
    $handler->display->display_options['fields']['worth']['table'] = 'field_data_worth';
    $handler->display->display_options['fields']['worth']['field'] = 'worth';
    $handler->display->display_options['fields']['worth']['label'] = 'Value';
    $handler->display->display_options['fields']['worth']['alter']['alter_text'] = TRUE;
    $handler->display->display_options['fields']['worth']['alter']['text'] = '-[worth]';
    $handler->display->display_options['fields']['worth']['click_sort_column'] = 'currcode';
    $handler->display->display_options['fields']['worth']['type'] = 'worth_field';
    /* Field: Transaction: Description */
    if ($fieldname = variable_get('transaction_description_field')) {
      $handler->display->display_options['fields'][$fieldname]['id'] = $fieldname;
      $handler->display->display_options['fields'][$fieldname]['table'] = 'field_data_'.$fieldname;
      $handler->display->display_options['fields'][$fieldname]['field'] = $fieldname;
      $handler->display->display_options['defaults']['arguments'] = FALSE;
    }

    $handler->display->display_options['defaults']['arguments'] = FALSE;
    /* Contextual filter: Transaction: Payer */
    $handler->display->display_options['arguments']['payer']['id'] = 'payer';
    $handler->display->display_options['arguments']['payer']['table'] = 'mcapi_transactions';
    $handler->display->display_options['arguments']['payer']['field'] = 'payer';
    $handler->display->display_options['arguments']['payer']['default_action'] = 'default';
    $handler->display->display_options['arguments']['payer']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['payer']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['payer']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['payer']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['payer']['summary_options']['items_per_page'] = '25';
    /* Contextual filter: Date: Date (mcapi_transactions) */
    $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
    $handler->display->display_options['arguments']['date_argument']['table'] = 'mcapi_transactions';
    $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
    $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
    $handler->display->display_options['arguments']['date_argument']['year_range'] = '-10:+0';
    $handler->display->display_options['arguments']['date_argument']['granularity'] = 'year';
    $handler->display->display_options['arguments']['date_argument']['use_fromto'] = 'no';
    $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
      'mcapi_transactions.created' => 'mcapi_transactions.created',
    );
    $handler->display->display_options['displays'] = array(
      'page' => 'page',
      'default' => 0,
    );
    $handler->display->display_options['attachment_position'] = 'after';
    $views[$view->name] = $view;
  }
  else {
    if ($GLOBALS['user']->uid == 1) drupal_set_message('Install date_views module for better paging', 'warning');
  }


  $view = new view;
  $view->name = 'user_transactions_summaries';
  $view->description = 'per currency transaction summary for individual users';
  $view->tag = 'mcapi (index)';
  $view->base_table = 'mcapi_index';
  $view->human_name = 'User transaction summaries';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'transact';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  if ($multicurrencies) {
    /* Field: Transaction index: Currency */
    $handler->display->display_options['fields']['currcode']['id'] = 'currcode';
    $handler->display->display_options['fields']['currcode']['table'] = 'mcapi_index';
    $handler->display->display_options['fields']['currcode']['field'] = 'currcode';
    $handler->display->display_options['fields']['currcode']['label'] = '';
    $handler->display->display_options['fields']['currcode']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['currcode']['alter']['text'] = '<strong>[currcode]</strong>';
    $handler->display->display_options['fields']['currcode']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['external'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['currcode']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['currcode']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['currcode']['alter']['html'] = 0;
    $handler->display->display_options['fields']['currcode']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['currcode']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['currcode']['hide_empty'] = 0;
    $handler->display->display_options['fields']['currcode']['empty_zero'] = 0;
    $handler->display->display_options['fields']['currcode']['hide_alter_empty'] = 0;
  }
  /* Field: SUM(Transaction index: Value) */
  $handler->display->display_options['fields']['diff']['id'] = 'diff';
  $handler->display->display_options['fields']['diff']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['diff']['field'] = 'diff';
  $handler->display->display_options['fields']['diff']['group_type'] = 'sum';
  $handler->display->display_options['fields']['diff']['label'] = 'Balance';
  $handler->display->display_options['fields']['diff']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['diff']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['diff']['hide_empty'] = 0;
  $handler->display->display_options['fields']['diff']['empty_zero'] = 0;
  $handler->display->display_options['fields']['diff']['hide_alter_empty'] = 0;
  /* Field: COUNT(User: Uid) in fact this could be any field because we are just counting result rows */
  $handler->display->display_options['fields']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['fields']['uid_1']['table'] = 'users';
  $handler->display->display_options['fields']['uid_1']['field'] = 'uid1';
  $handler->display->display_options['fields']['uid_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['uid_1']['label'] = 'Trades';
  $handler->display->display_options['fields']['uid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['uid_1']['format_plural'] = 0;
  /* Field: SUM(Transaction index: Expenditure) */
  $handler->display->display_options['fields']['expenditure']['id'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['expenditure']['field'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['group_type'] = 'sum';
  $handler->display->display_options['fields']['expenditure']['label'] = 'Gross expenditure';
  $handler->display->display_options['fields']['expenditure']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['expenditure']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['expenditure']['hide_empty'] = 0;
  $handler->display->display_options['fields']['expenditure']['empty_zero'] = 0;
  /* Field: SUM(Transaction index: Income) */
  $handler->display->display_options['fields']['income']['id'] = 'income';
  $handler->display->display_options['fields']['income']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['income']['field'] = 'income';
  $handler->display->display_options['fields']['income']['group_type'] = 'sum';
  $handler->display->display_options['fields']['income']['label'] = 'Gross income';
  $handler->display->display_options['fields']['income']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['income']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['income']['hide_empty'] = 0;
  $handler->display->display_options['fields']['income']['empty_zero'] = 0;
  /* Field: COUNT(DISTINCT Transaction index: 2nd user) */
  $handler->display->display_options['fields']['uid2']['id'] = 'uid2';
  $handler->display->display_options['fields']['uid2']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['uid2']['field'] = 'uid2';
  $handler->display->display_options['fields']['uid2']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['uid2']['label'] = 'Promiscuity';
  $handler->display->display_options['fields']['uid2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uid2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid2']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['uid2']['format_plural'] = 0;
  /* Contextual filter: Transaction index: 1st user */
  $handler->display->display_options['arguments']['uid1']['id'] = 'uid1';
  $handler->display->display_options['arguments']['uid1']['table'] = 'mcapi_index';
  $handler->display->display_options['arguments']['uid1']['field'] = 'uid1';
  $handler->display->display_options['arguments']['uid1']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid1']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['uid1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid1']['summary_options']['items_per_page'] = '25';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'user_summ_per_curr');
  $handler->display->display_options['block_description'] = 'User transactions summary per currency';
  $views[$view->name] = $view;



  $view = new view;
  $view->name = 'user_rankings';
  $view->description = 'If you have multiple currencies, be sure to change the argument. Also you can add \'between\' dates';
  $view->tag = 'mcapi (index)';
  $view->base_table = 'mcapi_index';
  $view->human_name = 'User rankings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Most trades';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['type'] = 'ol';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['row_options']['separator'] = ': ';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Relationship: Transaction index: 1st user */
  $handler->display->display_options['relationships']['uid1']['id'] = 'uid1';
  $handler->display->display_options['relationships']['uid1']['table'] = 'mcapi_index';
  $handler->display->display_options['relationships']['uid1']['field'] = 'uid1';
  $handler->display->display_options['relationships']['uid1']['required'] = 1;
  /* Relationship: Transaction index: 2nd user */
  $handler->display->display_options['relationships']['uid2']['id'] = 'uid2';
  $handler->display->display_options['relationships']['uid2']['table'] = 'mcapi_index';
  $handler->display->display_options['relationships']['uid2']['field'] = 'uid2';
  $handler->display->display_options['relationships']['uid2']['required'] = 1;
  if ($multicurrencies > 1) {
    /* Contextual filter: Transaction index: Currency */
    $handler->display->display_options['arguments']['currcode']['id'] = 'currcode';
    $handler->display->display_options['arguments']['currcode']['table'] = 'mcapi_index';
    $handler->display->display_options['arguments']['currcode']['field'] = 'currcode';
    $handler->display->display_options['arguments']['currcode']['default_action'] = 'default';
    $handler->display->display_options['arguments']['currcode']['default_argument_type'] = 'php';
    $handler->display->display_options['arguments']['currcode']['default_argument_options']['code'] = '$currencies = &drupal_static("currencies");return array_shift(array_keys($currencies));';
    $handler->display->display_options['arguments']['currcode']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['currcode']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['currcode']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['currcode']['summary_options']['items_per_page'] = '25';
  }

  /* Display: Promiscuity */
  $handler = $view->new_display('block', 'Promiscuity', 'block_promiscuity');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Promiscuity';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: COUNT(DISTINCT User: Uid) */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid2';
  $handler->display->display_options['fields']['uid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['text'] = '[uid] partners';
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['uid']['format_plural'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: COUNT(DISTINCT User: Uid) */
  $handler->display->display_options['sorts']['uid']['id'] = 'uid';
  $handler->display->display_options['sorts']['uid']['table'] = 'users';
  $handler->display->display_options['sorts']['uid']['field'] = 'uid';
  $handler->display->display_options['sorts']['uid']['relationship'] = 'uid2';
  $handler->display->display_options['sorts']['uid']['group_type'] = 'count_distinct';
  $handler->display->display_options['sorts']['uid']['order'] = 'DESC';

  /* Display: Volume */
  $handler = $view->new_display('block', 'Volume', 'block_volumes');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Volume';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: SUM(Transaction index: Volume) */
  $handler->display->display_options['fields']['volume']['id'] = 'volume';
  $handler->display->display_options['fields']['volume']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['volume']['field'] = 'volume';
  $handler->display->display_options['fields']['volume']['group_type'] = 'sum';
  $handler->display->display_options['fields']['volume']['label'] = '';
  $handler->display->display_options['fields']['volume']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['volume']['alter']['text'] = '[volume] traded';
  $handler->display->display_options['fields']['volume']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['volume']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['volume']['hide_empty'] = 0;
  $handler->display->display_options['fields']['volume']['empty_zero'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: SUM(Transaction index: Volume) */
  $handler->display->display_options['sorts']['volume']['id'] = 'volume';
  $handler->display->display_options['sorts']['volume']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['volume']['field'] = 'volume';
  $handler->display->display_options['sorts']['volume']['group_type'] = 'sum';
  $handler->display->display_options['sorts']['volume']['order'] = 'DESC';

  /* Display: Trades */
  $handler = $view->new_display('block', 'Trades', 'block_trades');
  $handler->display->display_options['display_description'] = 'Number of trades';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: COUNT(User: Name) */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['name_1']['label'] = '';
  $handler->display->display_options['fields']['name_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['text'] = '[name_1] trades';
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name_1']['format_plural'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: COUNT(User: Name) */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'uid1';
  $handler->display->display_options['sorts']['name']['group_type'] = 'count';
  $handler->display->display_options['sorts']['name']['order'] = 'DESC';

  /* Display: Incomes */
  $handler = $view->new_display('block', 'Incomes', 'block_incomes');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Incomes';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: SUM(Transaction index: Income) */
  $handler->display->display_options['fields']['income']['id'] = 'income';
  $handler->display->display_options['fields']['income']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['income']['field'] = 'income';
  $handler->display->display_options['fields']['income']['group_type'] = 'sum';
  $handler->display->display_options['fields']['income']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['income']['alter']['text'] = '[income] income';
  $handler->display->display_options['fields']['income']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['income']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['income']['hide_empty'] = 0;
  $handler->display->display_options['fields']['income']['empty_zero'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: SUM(Transaction index: Income) */
  $handler->display->display_options['sorts']['income']['id'] = 'income';
  $handler->display->display_options['sorts']['income']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['income']['field'] = 'income';
  $handler->display->display_options['sorts']['income']['group_type'] = 'sum';
  $handler->display->display_options['sorts']['income']['order'] = 'DESC';

  /* Display: Expenditure */
  $handler = $view->new_display('block', 'Expenditure', 'block_expenditures');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'expenditures';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: SUM(Transaction index: Expenditure) */
  $handler->display->display_options['sorts']['expenditure']['id'] = 'expenditure';
  $handler->display->display_options['sorts']['expenditure']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['expenditure']['field'] = 'expenditure';
  $handler->display->display_options['sorts']['expenditure']['group_type'] = 'sum';
  $handler->display->display_options['sorts']['expenditure']['order'] = 'DESC';

  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: SUM(Transaction index: Expenditure) */
  $handler->display->display_options['fields']['expenditure']['id'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['expenditure']['field'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['group_type'] = 'sum';
  $handler->display->display_options['fields']['expenditure']['label'] = '';
  $handler->display->display_options['fields']['expenditure']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['expenditure']['alter']['text'] = '[expenditure] spent';
  $handler->display->display_options['fields']['expenditure']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expenditure']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['expenditure']['hide_empty'] = 0;
  $handler->display->display_options['fields']['expenditure']['empty_zero'] = 0;
  /* Sort criterion: SUM(Transaction index: Expenditure) */
  $handler->display->display_options['sorts']['expenditure']['id'] = 'expenditure';
  $handler->display->display_options['sorts']['expenditure']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['expenditure']['field'] = 'expenditure';
  $handler->display->display_options['sorts']['expenditure']['group_type'] = 'sum';
  $handler->display->display_options['sorts']['expenditure']['order'] = 'DESC';
  $translatables['user_rankings'] = array(
    t('Most trades'),
    t('User rankings'),
    t('Promiscuity'),
    t('Volume'),
    t('Trades'),
    t('Number of trades'),
    t('Incomes'),
    t('Income'),
    t('Expenditure'),
    t('expenditures'),
    t('[volume] traded'),
    t('[name_1] trades'),
    t('[income] income'),
    t('[expenditure] spent'),
  );

  $views[$view->name] = $view;
  return $views;
}


  $translatables['user_transactions_summaries'] = array(
    t('Offset'),
    t('Promiscuity'),
    t('Gross expenditure'),
    t('Gross income'),
    t('Trades'),
    t('Currency'),
    t('Balance'),
    t('Running balance'),
  );
