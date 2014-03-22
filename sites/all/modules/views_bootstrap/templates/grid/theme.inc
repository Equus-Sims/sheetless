<?php

/**
 * Implementation of template preprocess for the view.
 */
function template_preprocess_views_bootstrap_grid_plugin_style(&$vars) {
  if ($columns = $vars['view']->style_options['columns']) {
    $vars['column_type'] = 12 / $vars['view']->style_options['columns'];
    $vars['columns'] = array_chunk($vars['rows'], $columns);
  }
}
