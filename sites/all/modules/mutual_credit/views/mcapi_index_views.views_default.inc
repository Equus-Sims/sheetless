<?php
//Id;

function mcapi_index_views_views_default_views() {
  currencies_load(); //because this seems to run before mcapi_init()
  $currencies = currencies_load();
  $multicurrencies = count($currencies) > 1;

  $view = new view();
  $view->name = 'statement';
  $view->description = 'shows trades of one user, with otheruser field and running balance. does not show dependent transactions';
  $view->tag = 'mcapi (index)';
  $view->base_table = 'mcapi_index';
  $view->human_name = 'Statement';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Statement';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'transact';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  if (module_exists('date_views')) {
    $handler->display->display_options['pager']['type'] = 'date_views_pager';
    $handler->display->display_options['pager']['options']['pager_position'] = 'bottom';
  }
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'empty text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = t('No transactions');
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: Transaction index: 2nd user */
  $handler->display->display_options['relationships']['uid2']['id'] = 'uid2';
  $handler->display->display_options['relationships']['uid2']['table'] = 'mcapi_index';
  $handler->display->display_options['relationships']['uid2']['field'] = 'uid2';
  $handler->display->display_options['relationships']['uid2']['required'] = TRUE;
  /* Field: Transaction index: Unique serial number */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['link_to_transaction'] = 0;
  /* Field: Transaction index: Created moment */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = t('Created');
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid2';
  $handler->display->display_options['fields']['name']['label'] = 'With';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Transaction: Description */
  $handler->display->display_options['fields']['transaction_description']['id'] = 'transaction_description';
  $handler->display->display_options['fields']['transaction_description']['table'] = 'field_data_transaction_description';
  $handler->display->display_options['fields']['transaction_description']['field'] = 'transaction_description';
  $handler->display->display_options['fields']['transaction_description']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['transaction_description']['alter']['path'] = 'transaction/[serial]';
  /* Field: Transaction index: Expenditure */
  $handler->display->display_options['fields']['expenditure']['id'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['expenditure']['field'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['hide_empty'] = TRUE;
  /* Field: Transaction index: Income */
  $handler->display->display_options['fields']['income']['id'] = 'income';
  $handler->display->display_options['fields']['income']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['income']['field'] = 'income';
  $handler->display->display_options['fields']['income']['hide_empty'] = TRUE;
  /* Field: Transaction index: Balance */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  $handler->display->display_options['fields']['balance']['hide_alter_empty'] = FALSE;

  /* Sort criterion: Transaction index: Created moment */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Transaction index: 1st user */
  $handler->display->display_options['arguments']['uid1']['id'] = 'uid1';
  $handler->display->display_options['arguments']['uid1']['table'] = 'mcapi_index';
  $handler->display->display_options['arguments']['uid1']['field'] = 'uid1';
  $handler->display->display_options['arguments']['uid1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid1']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid1']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid1']['summary_options']['items_per_page'] = '25';
  if (module_exists('date_views')) {
    /* Contextual filter: Date: Date (mcapi_index) */
    $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
    $handler->display->display_options['arguments']['date_argument']['table'] = 'mcapi_index';
    $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
    $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
    $handler->display->display_options['arguments']['date_argument']['year_range'] = '-10:+0';
    $handler->display->display_options['arguments']['date_argument']['granularity'] = 'year';
    $handler->display->display_options['arguments']['date_argument']['use_fromto'] = 'no';
    $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
      'mcapi_index.created' => 'mcapi_index.created',
    );
  }
  if ($multicurrencies) {
    /* Filter criterion: Transaction index: Currency */
    $handler->display->display_options['filters']['currcode']['id'] = 'currcode';
    $handler->display->display_options['filters']['currcode']['table'] = 'mcapi_index';
    $handler->display->display_options['filters']['currcode']['field'] = 'currcode';
    $handler->display->display_options['filters']['currcode']['exposed'] = TRUE;
    $handler->display->display_options['filters']['currcode']['expose']['operator_id'] = '';
    $handler->display->display_options['filters']['currcode']['expose']['label'] = t('Currency');
    $handler->display->display_options['filters']['currcode']['expose']['operator'] = 'currcode_op';
    $handler->display->display_options['filters']['currcode']['expose']['identifier'] = 'currcode';
    $handler->display->display_options['filters']['currcode']['expose']['reduce'] = 0;
  }

  /* Display: Page */
  $handler = $view->new_display('page', 'Statement', 'page');
  $handler->display->display_options['path'] = 'user/%/statement';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = t('Statement');
  $handler->display->display_options['menu']['weight'] = '0';

  $views[$view->name] = $view;


  $view = new view;
  $view->name = 'trade_analysis';
  $view->description = '';
  $view->tag = 'mcapi (index)';
  $view->base_table = 'mcapi_index';
  $view->human_name = 'Trading analysis';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = '';
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: COUNT(DISTINCT Transaction index: Unique serial number) */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['serial']['label'] = t('Trades');
  /* Champ: COUNT(DISTINCT Transaction index : 2nd user) */
  $handler->display->display_options['fields']['uid2']['id'] = 'uid2';
  $handler->display->display_options['fields']['uid2']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['uid2']['field'] = 'uid2';
  $handler->display->display_options['fields']['uid2']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['uid2']['label'] = t('Active traders');
  $handler->display->display_options['fields']['uid2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uid2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid2']['hide_alter_empty'] = 1;
  /* Champ: SUM(Transaction index : Volume) */
  $handler->display->display_options['fields']['volume']['id'] = 'volume';
  $handler->display->display_options['fields']['volume']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['volume']['field'] = 'volume';
  $handler->display->display_options['fields']['volume']['group_type'] = 'sum';
  $handler->display->display_options['fields']['volume']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['volume']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['volume']['hide_empty'] = 0;
  $handler->display->display_options['fields']['volume']['empty_zero'] = 0;
  $handler->display->display_options['fields']['volume']['hide_alter_empty'] = 1;
  /* Field: Transaction index: Greco index */
  $handler->display->display_options['fields']['greco']['id'] = 'greco';
  $handler->display->display_options['fields']['greco']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['greco']['field'] = 'greco';
  $handler->display->display_options['fields']['greco']['label'] = '';
  $handler->display->display_options['fields']['greco']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['greco']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['greco']['alter']['text'] = 'Greco index [greco]';
  $handler->display->display_options['fields']['greco']['element_label_colon'] = FALSE;

  /* Filter criterion: Transaction index: Currency */
  //just do this for the default currency - as an exposed filter it doesn't seem to work
  $handler->display->display_options['filters']['currcode']['id'] = 'currcode';
  $handler->display->display_options['filters']['currcode']['table'] = 'mcapi_index';
  $handler->display->display_options['filters']['currcode']['field'] = 'currcode';
  $handler->display->display_options['filters']['currcode']['value'] = array(key($currencies));

  /* Filter criterion: Transaction index: Value */
  $handler->display->display_options['filters']['diff']['id'] = 'diff';
  $handler->display->display_options['filters']['diff']['table'] = 'mcapi_index';
  $handler->display->display_options['filters']['diff']['field'] = 'diff';
  $handler->display->display_options['filters']['diff']['operator'] = '>';
  $handler->display->display_options['filters']['diff']['value']['value'] = '0';

  /* Filter criterion: Date: Date (mcapi_index) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'mcapi_index';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>';
  $handler->display->display_options['filters']['date_filter']['default_date'] = '-365 days';
  $handler->display->display_options['filters']['date_filter']['year_range'] = '-3:+0';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'mcapi_index.created' => 'mcapi_index.created',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'System trading stats', 'system');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'curr';
  $handler->display->display_options['access']['mode'] = 'system_data';
  $handler->display->display_options['block_description'] = 'Trading summary';

  /* Display: Bloc */
  $handler = $view->new_display('block', 'User trading stats', 'user');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'curr';
  $handler->display->display_options['access']['mode'] = 'user_data';
  $handler->display->display_options['title'] = 'User\'s trading summary';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relation: Transaction index : 1st user */
  $handler->display->display_options['relationships']['uid1']['id'] = 'uid1';
  $handler->display->display_options['relationships']['uid1']['table'] = 'mcapi_index';
  $handler->display->display_options['relationships']['uid1']['field'] = 'uid1';
  $handler->display->display_options['relationships']['uid1']['required'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: COUNT(DISTINCT Transaction index: Unique serial number) */
  $handler->display->display_options['fields']['serial']['id'] = 'serial';
  $handler->display->display_options['fields']['serial']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['serial']['field'] = 'serial';
  $handler->display->display_options['fields']['serial']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['serial']['label'] = 'Trades';
  /* Field: COUNT(DISTINCT Transaction index: 2nd user) */
  $handler->display->display_options['fields']['uid2']['id'] = 'uid2';
  $handler->display->display_options['fields']['uid2']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['uid2']['field'] = 'uid2';
  $handler->display->display_options['fields']['uid2']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['uid2']['label'] = 'Promiscuity';
  /* Field: SUM(Transaction index: Volume) */
  $handler->display->display_options['fields']['volume']['id'] = 'volume';
  $handler->display->display_options['fields']['volume']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['volume']['field'] = 'volume';
  $handler->display->display_options['fields']['volume']['group_type'] = 'sum';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Filtre contextuel: Transaction index : 1st user */
  $handler->display->display_options['arguments']['uid1']['id'] = 'uid1';
  $handler->display->display_options['arguments']['uid1']['table'] = 'mcapi_index';
  $handler->display->display_options['arguments']['uid1']['field'] = 'uid1';
  $handler->display->display_options['arguments']['uid1']['title_enable'] = 1;
  $handler->display->display_options['arguments']['uid1']['title'] = '%1 summary';
  $handler->display->display_options['arguments']['uid1']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid1']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['uid1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid1']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid1']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid1']['not'] = 0;

  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;

  $views[$view->name] = $view;
  return $views;
}


  $translatables['system_stats'] = array(
    t('User\'s trading summary'),
    t('Trading analysis'),
    t('Trades'),
    t('Active traders'),
    t('Volume'),
    t('Promiscuity'),
    t('%1 summary'),
    t('1st user'),
    t('2nd user'),
    t('Balance'),
    t('Name'),
    t('Currency'),
    t('Statement'),
    t('Greco index [greco]')
  );
