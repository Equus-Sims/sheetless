<?php

/**
 * @file
 * Contains views templates on behalf of the mcapi_balance_history module.
 */
function mcapi_balance_history_views_templates() {
  $view = new view();
  $view->name = 'nudgeneeders';
  $view->description = 'Users by the extent of their balance, with chart';
  $view->tag = 'default';
  $view->base_table = 'mcapi_index';
  $view->human_name = t('Users needing nudges');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'balance_history' => 'balance_history',
    'diff' => 'diff',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'balance_history' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'diff' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  /* Relationship: Transaction index: 1st user */
  $handler->display->display_options['relationships']['uid1']['id'] = 'uid1';
  $handler->display->display_options['relationships']['uid1']['table'] = 'mcapi_index';
  $handler->display->display_options['relationships']['uid1']['field'] = 'uid1';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid1';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Field: Transaction index: Balance_history */
  $handler->display->display_options['fields']['balance_history']['id'] = 'balance_history';
  $handler->display->display_options['fields']['balance_history']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['balance_history']['field'] = 'balance_history';
  $handler->display->display_options['fields']['balance_history']['label'] = '';
  $handler->display->display_options['fields']['balance_history']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['balance_history']['width'] = '300';
  $handler->display->display_options['fields']['balance_history']['height'] = '100';
  $handler->display->display_options['fields']['balance_history']['since'] = '-1 year';
  /* Field: SUM(Transaction index: Change in balance) */
  $handler->display->display_options['fields']['diff']['id'] = 'diff';
  $handler->display->display_options['fields']['diff']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['diff']['field'] = 'diff';
  $handler->display->display_options['fields']['diff']['group_type'] = 'sum';
  $handler->display->display_options['fields']['diff']['label'] = 'Balance';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h3>Need help earning...</h3>';
  $handler->display->display_options['header']['area']['format'] = 'editor_filtered_html';
  $handler->display->display_options['path'] = 'nudgeneeders';

  /* Display: Attachment */
  $handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h3>Need help spending...</h3>';
  $handler->display->display_options['header']['area']['format'] = 'editor_filtered_html';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: SUM(Transaction index: Change in balance) */
  $handler->display->display_options['sorts']['diff']['id'] = 'diff';
  $handler->display->display_options['sorts']['diff']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['diff']['field'] = 'diff';
  $handler->display->display_options['sorts']['diff']['group_type'] = 'sum';
  $handler->display->display_options['sorts']['diff']['order'] = 'DESC';
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'default' => 0,
  );
  $handler->display->display_options['attachment_position'] = 'after';
  $translatables['test'] = array(
    t('Need help earning...'),
    t('Need help spending...'),
  );

  $views[$view->name] = $view;

  return $views;
}
