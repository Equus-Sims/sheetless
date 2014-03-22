<?php

/**
 * Implementation of template preprocess for the view.
 */
function template_preprocess_views_bootstrap_accordion_plugin_style(&$vars) {
  $view = &$vars['view'];
  $title_field = $vars['options']['title_field'];

  // Get titles.
  if (isset($view->field[$title_field])) {
    foreach ($vars['view']->result as $key => $field) {
      $vars['titles'][$key] = trim($view->field[$title_field]->get_value($field));
    }
  }
}
