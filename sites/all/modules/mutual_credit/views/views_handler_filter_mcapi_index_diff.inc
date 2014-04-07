<?php
/**
 * Simple filter to handle matching of boolean values.
 *
 * This handler checks to see if a string field is empty (equal to '') or not.
 * It is otherwise identical to the parent operator.
 *
 *
 */
class views_handler_filter_mcapi_index_diff extends views_handler_filter_numeric {

  function operators() {
    $operators = array(
      '=' => array(
        'title' => t('None'),
        'short' => t('none'),
        'values' => 0,
        'ensure_my_table' => TRUE
      ),
      '>' => array(
        'title' => t('Incoming'),
        'short' => t('incoming'),
        'values' => 0,
        'ensure_my_table' => TRUE
      ),
      '<' => array(
        'title' => t('Outgoing'),
        'short' => t('outgoing'),
        'values' => 0,
        'ensure_my_table' => TRUE
      )
    );
    return $operators;
  }


  function query() {
    if ($this->operator == '=') return;
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.diff", 0, $this->operator);
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    $form['diff_op']['#type'] = 'radios';
    unset($form['diff']);
  }

}