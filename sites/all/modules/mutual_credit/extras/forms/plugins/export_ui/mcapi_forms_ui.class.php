<?php
/*
 * extends isn't working, which makes this whole file useless
 */
class mcapi_forms_ui extends ctools_export_ui {

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
   * method, so this is building up each row suitable for theme('table').
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->{$this->plugin['export']['key']};

    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;
      case 'title':
        $this->sorts[$name] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->type . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    //first col, name
    $this->rows[$name]['data'][] = array('data' => check_plain($item->data['experience']['title']), 'class' => array('ctools-export-ui-name'));

    //second col, path
    $path = $item->data['architecture']['path'];
    if (!strpos($path, '%') && empty($item->disabled)) {
      $path = l($path, $path);
    }
    $this->rows[$name]['data'][] = array('data' => $path);
    //third col, storage
    $this->rows[$name]['data'][] = array('data' => check_plain($item->type), 'class' => array('ctools-export-ui-storage'));

    // Reorder the operations so that enable is the default action for a templatic views
    if (!empty($operations['enable'])) {
      $operations = array('enable' => $operations['enable']) + $operations;
    }
    //we might want to add a hidden field for this so forms can declare themselves undisableable
    if ($name == 'add_my_signature') {
      unset($operations['disable']);
    }

    if (module_exists('i18n_string')) {
      $operations['translate'] = array(
        'title' => t('Translate'),
        'href' => "admin/accounting/forms/list/$name/translate"
      );
    }
    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));
    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    //this is critical but I can't work out where it is supposed to go
    //after editing the form, it always returns to this page.
    cache_clear_all();
    menu_rebuild();
    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    //$header[] = array('data' => t('Help'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Path'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));
    return $header;
  }
}

