<?php
/**
 * @file
 * Definition of views_bootstrap_plugin_style.
 */

/**
 * Class to define a style plugin handler.
 */
class ViewsBootstrapGridPluginStyle extends views_plugin_style {
  /**
   * Definition.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['columns'] = array('default' => 2);
    return $options;
  }

  /**
   * Form.
   */
  function options_form(&$form, &$form_state) {
    $options = array(1, 2, 3, 4, 6, 12);
    $form['columns'] = array(
      '#type' => 'select',
      '#title' => t('Number of columns'),
      '#default_value' => $this->options['columns'],
      '#options' => array_combine($options, $options),
      '#required' => TRUE,
    );
  }
}
