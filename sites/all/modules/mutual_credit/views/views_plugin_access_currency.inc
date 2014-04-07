<?php


/**
 * Access plugin that provides permission-based access control.
 */

class views_plugin_access_currency extends views_plugin_access {

  function access($account) {
    //grant access only if none of the included currencies deny it
    $currcodes = array();
    foreach($this->view->display_handler->options['filters'] as $filter) {
      if($filter['field'] == 'currcode') {
        $currcodes = $filter['value'];
        break;
      }
    }
    foreach ($currcodes as $currcode) {
      if (!currency_access($this->options['mode'], $currcode, $account)) return FALSE;
    }
    return TRUE;
  }


  function option_definition() {
    $options = parent::option_definition();
    $options['mode'] = array('default' => 'membership');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['mode'] = array(
      '#title' => t('Currency access mode'),
      '#description' => t("See the settings on each currency's page."),
      '#type' => 'radios',
      '#options' => array(
        'membership'  => t("Users who have '@perm' for all shown currencies", array('@perm' => t('Use the currency'))),
        'trader_data' => t("Users who have '@perm' for all shown currencies", array('@perm' => t('View aggregated user transaction data'))),
        'system_data' => t("Users who have '@perm' for all shown currencies", array('@perm' => t('View aggregated system data'))),
      ),
      '#default_value' => $this->options['mode']
    );

  }


  function get_access_callback() {
    if (array_key_exists('worth_currcode', $this->view->filter)) {
      $currcode = $this->view->filter['worth_currcode']->value;
    }
    elseif(array_key_exists('worth_currcode', $this->view->argument)) {
      $currcode = $this->view->argument['worth_currcode']->value;
      if (!$currcode) {
        if ($this->view->argument['worth_currcode']->options['default_argument_type'] = 'fixed') {//get the default
          $currcode = $this->view->argument['worth_currcode']->options['default_argument_options']['argument'];
        }
        else $currcode = 'credunit';
      }
    }
    ////this a hack because one site's currency filter
    //  seems to extend the wrong class
    if (is_array($currcode))$currcode = key($currcode);
    return array('currency_access', array($this->options['mode'], $currcode));
  }

  function summary_title() {
    switch($this->options['mode']) {
      case 'membership': return t('Membership');
      case 'trader_data': return t('User summary data');
      case 'system_data': return t('System-wide summary data');
    }
  }
}