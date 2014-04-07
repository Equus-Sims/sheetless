<?php
/**
 * @file
 * Contains the basic 'transaction' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a transaction.
 * Definition terms:
 * - link_to_transaction default: Should this field have the checkbox "link to transaction" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_transaction extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_transaction'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to transaction option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_transaction'] = array(
      '#title' => t('Link this field to the transaction'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_transaction']),
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    //TODO for some reason the serial number wasn't added as an additional field
    $serial = &$values->{$this->aliases['serial']};
    if (!$this->options['link_to_transaction']) return $serial;

    return array(
      '#theme' => 'link',
      '#text' => $serial,
      '#path' => 'transaction/'.$serial,
      '#options' => array(
        'attributes' =>array(),
        'html' => TRUE
      )
    );
  }
}
