<?php

class mcapi_ui extends ctools_export_ui {

  function init($plugin) {
    $prefix_count = count(explode('/', $plugin['menu']['menu prefix']));
    $plugin['menu']['items']['add-template'] = array(
      'path' => 'template/%/add',
      'title' => 'Add from template',
      'page callback' => 'ctools_export_ui_switcher_page',
      'page arguments' => array($plugin['name'], 'add_template', $prefix_count + 2),
      'load arguments' => array($plugin['name']),
      'access callback' => 'ctools_export_ui_task_access',
      'access arguments' => array($plugin['name'], 'add_template', $prefix_count + 2),
      'type' => MENU_CALLBACK,
    );
    return parent::init($plugin);
  }

  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   *
   * but what I want, is to add the object->path, as a link, to the table
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $currcode = &$item->{$this->plugin['export']['key']};

    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$currcode] = empty($item->disabled) . $currcode;
        break;
      case 'title':
        $this->sorts[$currcode] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$currcode] = $currcode;
        break;
      case 'storage':
        $this->sorts[$currcode] = $item->type . $currcode;
        break;
    }
    $this->rows[$currcode]['data'] = array();
    $this->rows[$currcode]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');
    //first col, Name
    $this->rows[$currcode]['data'][1] = array(
      'data' => check_plain($item->data->human_name),
      'class' => array('ctools-export-ui-title')
    );
    $type_names = array(
      CURRENCY_TYPE_ACKNOWLEDGEMENT => t('Acknowledgement'),
      CURRENCY_TYPE_EXCHANGE => t('Exchange'),
      CURRENCY_TYPE_COMMODITY => t('Commodity')
    );
    //second col, issuance
    $type = property_exists($item->data, 'issuance') ? $item->data->issuance : CURRENCY_TYPE_ACKNOWLEDGEMENT;
    $this->rows[$currcode]['data'][2] = $type_names[$type];
    //third col, usage
    $this->rows[$currcode]['data'][3] = array(
      'data' => count(transaction_filter(array('currcode' => $item->currcode)))
    );
    //fourth col, storage
    $this->rows[$currcode]['data'][4] = array(
      'data' => check_plain($item->type),
      'class' => array('ctools-export-ui-storage')
    );
    //final col, links
    $ops = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline'))
    );
    $this->rows[$currcode]['data'][5] = array(
      'data' => $ops,
      'class' => array('ctools-export-ui-operations')
    );
    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$currcode]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    //yuk, I don't know how views clears the cache, but this is as a good a place to do it as any.
    cache_clear_all('currencies', 'cache');

    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }
    else{
      $header[] = array('data' => t('Currency code'), 'class' => array('ctools-export-ui-name'));
    }

    $header[] = array('data' => t('Issuance'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Transactions'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

  function list_page($js, $input) {
    $this->items = ctools_export_crud_load_all($this->plugin['schema'], $js);
        // This is where the form will put the output.
    $this->rows = array();
    $this->sorts = array();

    $form_state = array(
      'plugin' => $this->plugin,
      'input' => $input,
      'rerender' => TRUE,
      'no_redirect' => TRUE,
      'object' => &$this,
    );

    if (!isset($form_state['input']['form_id'])) {
      $form_state['input']['form_id'] = 'ctools_export_ui_list_form';
    }
    //this populates $this->rows
    $form = drupal_build_form('ctools_export_ui_list_form', $form_state);
    return $this->list_render($form_state);
  }


  //replacing the ctools function because we need to ensure that the last enabled currency cannot be disabled
  //so its all about the allowed operations and disabling the ajax
  function list_form_submit(&$form, &$form_state) {
    // Filter and re-sort the pages.
    $plugin = $this->plugin;
    $schema = ctools_export_get_schema($this->plugin['schema']);

    $prefix = ctools_export_ui_plugin_base_path($plugin);

    //matslats
    foreach ($this->items as $currcode => $item) {
      if (empty($item->disabled)) $enabled[] = $currcode;
    }
    foreach ($this->items as $currcode => $item) {
      // Call through to the filter and see if we're going to render this
      // row. If it returns TRUE, then this row is filtered out.
      if ($this->list_filter($form_state, $item)) {
        continue;
      }
      // Note: Creating this list seems a little clumsy, but can't think of
      // better ways to do this.
      $allowed_operations = drupal_map_assoc(array_keys($plugin['allowed operations']));
      unset($allowed_operations['import']);
      unset($allowed_operations['enable']);
      $allowed_operations = array('enable' => 'enable') + $allowed_operations;


      if ($item->{$schema['export']['export type string']} == t('Normal')) {
        unset($allowed_operations['revert']);
      }
      elseif ($item->{$schema['export']['export type string']} == t('Overridden')) {
        unset($allowed_operations['delete']);
      }
      else {
        unset($allowed_operations['revert']);
        unset($allowed_operations['delete']);
      }
      //prevent the last enabled currency from being disabled or deleted
      if (count($enabled) == 1 && $enabled[0] == $currcode) {
        unset($allowed_operations['disable']);
        unset($allowed_operations['delete']);
      }
      //prevent both 'enable' and 'disable' being present
      if(!in_array($currcode, $enabled)) {
        unset($allowed_operations['disable']);
      }
      else {
        unset($allowed_operations['enable']);
      }
      //prevent it being deleted if there are any non-deleted transactions
      $used= db_query(
        "SELECT count(xid)
          FROM {mcapi_transactions} t LEFT JOIN {field_data_worth} w ON t.xid = w.entity_id
          WHERE t.state > 0 AND w.worth_currcode = :currcode", array(':currcode' => $currcode)
      )->fetchField();
      if ($used) {
        unset($allowed_operations['delete']);
      }
      $operations = array();

      foreach ($allowed_operations as $op) {
        $operations[$op] = array(
          'title' => $plugin['allowed operations'][$op]['title'],
          'href' => ctools_export_ui_plugin_menu_path($plugin, $op, $currcode),
        );
        if (!empty($plugin['allowed operations'][$op]['ajax'])) {
          //matslats
          //here we have disabled the ajax, forcing the page to reload
          //$operations[$op]['attributes'] = array('class' => array('use-ajax'));
        }
        if (!empty($plugin['allowed operations'][$op]['token'])) {
          $operations[$op]['query'] = array('token' => drupal_get_token($op));
        }
      }

      $this->list_build_row($item, $form_state, $operations);
    }

    // Now actually sort
    if ($form_state['values']['sort'] == 'desc') {
      arsort($this->sorts);
    }
    else {
      asort($this->sorts);
    }

    // Nuke the original.
    $rows = $this->rows;
    $this->rows = array();
    // And restore.
    foreach ($this->sorts as $name => $title) {
      $this->rows[$name] = $rows[$name];
    }
  }
}