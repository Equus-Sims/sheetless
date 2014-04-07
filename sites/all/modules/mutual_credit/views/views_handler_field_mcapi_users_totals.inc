<?php
// $Id:

/**
 * @file
 * Given the user, this shows the balance.
 * choose ONE currency from a list of options.
 * note that this is a bit of a pseudo field, since it makes a separate call to the transaction controller layer.
 */
class views_handler_field_mcapi_users_totals extends views_handler_field {

  var $fetched;

  function query() {
    $this->ensure_my_table();
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['currcode'] = array('default' => array());
    $options['total'] = array('default' => 'balance');
    $options['strip'] = array('default' => FALSE);
    $options['separator'] = array('default' => ', ');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $currencies = currencies_load();
    module_load_include('inc', 'mcapi');
    $form['currcode'] = currency_picker_element(array_keys($currencies), $this->options['currcode'], TRUE);
    $form['strip'] = array(
      '#title' => t('Strip formatting'),
      '#description' => t('Just show the unformatted quantity (best with only one currency)'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['strip']
    );
    $form['total'] = array(
      '#title' => t('Aggregate'),
      '#description' => t('How do you want to summarise the the transaction totals?'),
      '#type' => 'radios',
      '#options' => array(
        'count' => t('Number of transactions'),
        'gross_in' => t('Gross income'),
        'gross_out' => t('Gross expenditure'),
        'balance' => t('Balance'),
        'participation' => t('Participation')
      ),
      '#default_value' => $this->options['total']
    );
    if (count(currencies_load()) > 1) {
      $form['separator'] = array(
        '#title' => t('Separator'),
        '#description' => t('in case of more than one currency'),
        '#type' => 'textfield',
        '#size' => 10,
        '#default_value' => $this->options['separator']
      );
    }
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $uid = &$values->uid;
    //the settings imply there is only one currcode, but actually many can be rendered
    $currcodes = empty($this->options['currcode']) ? array_keys(currencies_load(user_load($uid))): (array)$this->options['currcode'];
    $elements = array();
    foreach ($currcodes as $currcode) {
      if (empty($this->fetched[$uid][$currcode])) {
        $this->fetched[$uid][$currcode] = transaction_totals($uid, $currcode);
      }
      $quant = $this->fetched[$uid][$currcode]->{$this->options['total']};
      if ($this->options['strip']) {
        $elements[$currcode][] = array('#type' => 'markup', '#markup' => $quant);
      }
      else {
        $elements[$currcode] = array(
          '#theme' => 'worth_item',
          '#quantity' => $quant,
          '#currcode' => $currcode,
          '#suffix' => $this->options['separator']
        );
      }
    }
    //remove the last sepator
    unset($elements[$currcode]['#suffix']);
    return $elements;
  }
}
