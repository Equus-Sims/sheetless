<?php

/**
 * Implementation of template preprocess for the view.
 */
function template_preprocess_views_bootstrap_tab_plugin_style(&$vars) {
  $view = &$vars['view'];
  $tab_field = $vars['options']['tab_field'];

  $vars['tab_type'] = $vars['options']['tab_type'];

  // Get tabs.
  if (isset($view->field[$tab_field])) {
    foreach ($vars['view']->result as $key => $field) {
      $vars['tabs'][$key] = trim($view->field[$tab_field]->get_value($field));
    }
  }
}
