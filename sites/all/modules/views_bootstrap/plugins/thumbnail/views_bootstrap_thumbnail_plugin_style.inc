<?php
/**
 * @file
 * Definition of views_bootstrap_plugin_style.
 */

/**
 * Class to define a style plugin handler.
 */
class ViewsBootstrapThumbnailPluginStyle extends views_plugin_style {
  /**
   * Definition.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['column_type'] = array('default' => 4);
    return $options;
  }

  /**
   * Form.
   */
  function options_form(&$form, &$form_state) {
    //@TODO: Grouping doesn't work here.
    //parent::options_form($form, $form_state);

    $form['column_type'] = array(
      '#type' => 'select',
      '#title' => t('Number of columns'),
      '#options' => array(
        12 => 1,
        6 => 2,
        4 => 3,
        3 => 4,
        2 => 6,
        1 => 12,
      ),
      '#default_value' => $this->options['column_type'],
    );
  }
}
