<?php
/**
 * Simple filter to handle matching of boolean values.
 *
 * This handler filters by the transaction state.
 * See mcapi_get_states() and the mcapi hook that declares states
 *
 */
class views_handler_filter_mcapi_states extends views_handler_filter_in_operator {

  function construct() {
    //$this->value_value = t('True');
    $this->value_value = t('State');
    $this->accept_null = FALSE;
    $this->value_options = NULL;
    parent::construct();
  }

  function get_value_options() {
    $this->value_options = mcapi_get_states('#options');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['value']['default'] = TRANSACTION_STATE_FINISHED;
    return $options;
  }


  function value_form(&$form, &$form_state) {
    if (empty($this->value_options)) {
      // Initialize the array of possible values for this filter.
      $this->get_value_options();
    }
    $form['value'] = array(
      '#title' => t('State'),
      '#description' => mcapi_get_states('#description'),
      '#type' => 'select',
      '#options' => $this->value_options,
      '#default_value' => $this->value,
      '#multiple' => TRUE
    );
    if (!empty($this->options['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
      // If we're configuring an exposed filter, add an <Any> option.
      if (empty($form_state['exposed']) || empty($this->options['required'])) {
        $any_label = variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? '<Any>' : t('- Any -');
        if ($form['value']['#type'] != 'select') {
          $any_label = check_plain($any_label);
        }
        $form['value']['#options'] = array('All' => $any_label) + $form['value']['#options'];
      }
    }
  }

  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
    if (empty($this->value_options)) {
      $this->get_value_options();
    }
    // Now that we have the valid options for this filter, just return the
    // human-readable label based on the current value.
    return implode(', ', array_intersect_key($this->value_options, $this->value));
  }

  function expose_options() {
    parent::expose_options();
    $this->options['expose']['operator_id'] = '';
    $this->options['expose']['label'] = t('State');
    $this->options['expose']['required'] = TRUE;
  }
}
