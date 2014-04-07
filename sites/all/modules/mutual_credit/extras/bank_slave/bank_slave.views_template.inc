<?php

function bank_slave_views_templates() {
  $view = new view();
  $view->name = 'bank';
  $view->description = 'Configure admin/accounting/bank before creating this view';
  $view->tag = 'default';
  $view->base_table = 'mcapi_index';
  $view->human_name = 'A bank statement for each account & currency.';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bank statement';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uid2' => 'uid2',
    'created' => 'created',
    'expenditure' => 'expenditure',
    'income' => 'income',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'uid2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expenditure' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'income' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Comportement en l'absence de résultats: Global : Zone de texte */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'norows';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No activity has been logged in this period';
  $handler->display->display_options['empty']['area']['format'] = 'editor_filtered_html';
  /* Relation: Index d'échanges : 2ième utilisateur */
  $handler->display->display_options['relationships']['uid2']['id'] = 'uid2';
  $handler->display->display_options['relationships']['uid2']['table'] = 'mcapi_index';
  $handler->display->display_options['relationships']['uid2']['field'] = 'uid2';
  /* Champ: Index d'échanges : Le moment de la création */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Champ: Echange : Description */
  $handler->display->display_options['fields']['transaction_description']['id'] = 'transaction_description';
  $handler->display->display_options['fields']['transaction_description']['table'] = 'field_data_transaction_description';
  $handler->display->display_options['fields']['transaction_description']['field'] = 'transaction_description';
  /* Champ: Index d'échanges : Unités données */
  $handler->display->display_options['fields']['expenditure']['id'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['expenditure']['field'] = 'expenditure';
  $handler->display->display_options['fields']['expenditure']['hide_empty'] = TRUE;
  /* Champ: Index d'échanges : Unités reçues */
  $handler->display->display_options['fields']['income']['id'] = 'income';
  $handler->display->display_options['fields']['income']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['income']['field'] = 'income';
  $handler->display->display_options['fields']['income']['hide_empty'] = TRUE;
  /* Champ: Index d'échanges : Solde */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'mcapi_index';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  /* Critère de tri: Index d'échanges : Le moment de la création */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'mcapi_index';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['granularity'] = 'day';
  /* Filtre contextuel: Date : Date (mcapi_index) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'mcapi_index';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument']['exception']['title'] = 'All';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['year_range'] = '-3:+0';
  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'year';
  $handler->display->display_options['arguments']['date_argument']['use_fromto'] = 'no';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'mcapi_index.created' => 'mcapi_index.created',
  );
  /* Critère de filtrage: Index d'échanges : Devise */
  $handler->display->display_options['filters']['currcode']['id'] = 'currcode';
  $handler->display->display_options['filters']['currcode']['table'] = 'mcapi_index';
  $handler->display->display_options['filters']['currcode']['field'] = 'currcode';
  $handler->display->display_options['filters']['currcode']['value'] = array('escrow' => 'escrow');

  $i = 0;
  foreach (variable_get('mcapi_bank_slave_uids', array()) as $bank_uid) {
    $bankname = user_load($bank_uid)->name;
    foreach (variable_get('mcapi_bank_slave_currcodes') as $currcode) {
      $currency = currency_load($currcode);
      $tabname = t("@name @currency", array('@name' => $bankname, '@currency' => $currency->human_name));
      $i++;
      $handler = $view->new_display('page', $tabname, 'page_'.$i);
      $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
      $handler->display->display_options['path'] = 'finances/'. urlencode($bankname) .'-'. $currcode;
      $handler->display->display_options['menu']['type'] = $i == 1 ? 'default tab' : 'tab';
      $handler->display->display_options['menu']['title'] = $tabname;
      $handler->display->display_options['menu']['weight'] = $i;
    //$handler->display->display_options['menu']['name'] = 'secondary-menu';
      if ($i == 1) {//the parent menu item
        $handler->display->display_options['tab_options']['type'] = 'normal';
        $handler->display->display_options['tab_options']['title'] = 'Finances';
        $handler->display->display_options['tab_options']['weight'] = 5;
      }
      $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
      $handler->display->display_options['defaults']['filter_groups'] = FALSE;
      $handler->display->display_options['defaults']['filters'] = FALSE;

      /* Critère de filtrage: Index d'échanges : Devise */
      $handler->display->display_options['filters']['currcode']['id'] = 'currcode';
      $handler->display->display_options['filters']['currcode']['table'] = 'mcapi_index';
      $handler->display->display_options['filters']['currcode']['field'] = 'currcode';
      $handler->display->display_options['filters']['currcode']['value'] = array($currcode => $currcode);
      $handler->display->display_options['filters']['currcode']['group'] = 1;
      /* Critère de filtrage: Index d'échanges : 1er utilisateur */
      $handler->display->display_options['filters']['uid1']['id'] = 'uid1';
      $handler->display->display_options['filters']['uid1']['table'] = 'mcapi_index';
      $handler->display->display_options['filters']['uid1']['field'] = 'uid1';
      $handler->display->display_options['filters']['uid1']['value']['value'] = $bank_uid;

    }
  }

  $translatables['bank'] = array(
    t('Configure admin/accounting/bank before creating this view'),
    t('A bank statement for each account & currency.'),
    t('Bank statement'),
    t('No activity has been logged in this period'),
    t('Date'),
    t('Description'),
    t('Deposit'),
    t('Withdrawal'),
    t('Balance'),
  );

  return array($view->name => $view);
}
