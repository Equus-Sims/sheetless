<?php

/**
 * @file
 * Implements file flags.
 */

/**
 * Implements a file flag.
 */
class flag_file extends flag_flag {
  function options() {
    $options = parent::options();
    $options += array(
      'show_on_file_page' => TRUE,
    );
    return $options;
  }

  function options_form(&$form) {
    parent::options_form($form);

    $options = array();
    foreach (file_info_file_types() as $name => $type) {
      $options[$name] = check_plain($type['label']);
    }
    $form['access']['types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('The file types whose files will be flaggable'),
      '#options' => $options,
      '#default_value' => $this->types,
      '#required' => TRUE,
      '#weight' => 10,
      '#access' => empty($flag->locked['types']),
    );
    $form['display']['show_on_file_page'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display link on file pages'),
      '#default_value' => $this->show_on_file_page,
      '#access' => empty($this->locked['show_on_file_page']),
    );
  }

  function _load_content($content_id) {
    return file_load($content_id);
  }

  function applies_to_content_object($file) {
    if ($file && in_array($file->type, $this->types)) {
      return TRUE;
    }
    return FALSE;
  }

  function get_content_id($file) {
    return $file->fid;
  }

  function uses_hook_link($teaser) {
    if ($this->show_on_file_page) {
      return TRUE;
    }
    return FALSE;
  }

  function get_labels_token_types() {
    return array('file');
  }

  function replace_tokens($label, $contexts, $options, $content_id) {
    if ($content_id && ($file = $this->fetch_content($content_id))) {
      $contexts['file'] = $file;
    }
    return parent::replace_tokens($label, $contexts, $options, $content_id);
  }

  function get_flag_action($content_id) {
    $flag_action = parent::get_flag_action($content_id);
    $file = $this->fetch_content($content_id);
    $flag_action->content_title = $file->filename;
    $flag_action->content_url = _flag_url('file/' . $file->fid);
    return $flag_action;
  }

  function get_relevant_action_objects($content_id) {
    return array(
      'file' => $this->fetch_content($content_id),
    );
  }

  function get_views_info() {
    return array(
      'views table' => 'file_managed',
      'join field' => 'fid',
      'title field' => 'filename',
      'title' => t('File flag'),
      'help' => t('Limit results to only those files flagged by a certain flag; Or display information about the flag set on a file.'),
      'counter title' => t('File flag counter'),
      'counter help' => t('Include this to gain access to the flag counter field.'),
    );
  }

  function applies_to_content_id_array($content_ids) {
    $passed = array();
    foreach ($content_ids as $fid) {
      if ($this->applies_to_content_id($fid)) {
        $passed[$fid] = TRUE;
      }
    }
    return $passed;
  }
}
