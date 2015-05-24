<?php
/**
 * @file
 * equus_properties.features.inc
 */

/**
 * Implements hook_node_info().
 */
function equus_properties_node_info() {
  $items = array(
    'property' => array(
      'name' => t('Property'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
