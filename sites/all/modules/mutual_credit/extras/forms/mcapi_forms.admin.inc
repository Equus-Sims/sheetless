<?php
include ('mcapi_forms.inc');

/**
 * implements hook_help
 */
function mcapi_forms_help($path, $args) {
  if (substr($path, 0, 22) == 'admin/accounting/forms') {
    return t('Create transaction forms for specific use-cases. Use with caution as not all permutations are meaningful.');
  }
}

/*
 * This is the form-builder form
 * For every field in the transaction form, this form allows it to be configured.
 * Its not geometrically perfect, but covers a lot of use cases without getting too complicated
 */
function _edit_mcapi_form(&$form, &$form_state) {
  $settings = &$form_state['item']->data;
  $form['#prefix'] = '<div id = "form-wrapper">';
  $form['#suffix'] = '</div>';
  //I think ctools is supposed to be handling this
  $form['info']['name']['#type'] = 'machine_name';
  $form['info']['name']['#machine_name']['exists'] = 'mcapi_forms_exists';
  $form['info']['name']['#maxlength'] = 32;
  $perspectives = array(
    3 => t('3rd Party'),
    1 => t('1st Party')
  );
  //to cover up the fact that we can only reload the form once
  if (isset($form_state['values']['perspective'])) {
    //only include the selected option
    $form['perspective']['#options'] = array($form_state['values']['perspective'] => $perspectives[$form_state['values']['perspective']]);
    $form['perspective']['#description'] = 'Technical limitations prohibit this being flipped more than once. Reload the whole page to change it back';
  }
  $menus = module_exists('menu') ? menu_get_menus() : menu_list_system_menus();
  $menus['navigation'] = t('Default');
  $form['architecture'] = array(
    '#title' => t('Architecture'),
    '#description' => t("Every form is a menu callback with its own access control. Don't forget currencies have their own access control as well."),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => !empty($settings),
    '#weight' => 3,
    'path' => array(
      '#title' => t('Path'),
      '#description' => t("Unique path to this form. Don't forget you can use a path alias over this"),
      '#type' => 'textfield',
      '#default_value' => isset($settings['architecture']['path']) ? $settings['architecture']['path'] : '',
      '#required' => TRUE,
      '#weight' => 1,
      '#element_validate' => array('mcapi_element_validate_path_duplicate')
    ),
    'menu_link' =>array(
      '#title' => t('Menu link location'),
      '#description' => t('Choose the menu, or menu item-type.') .' '. t('Some choices may not produce a menu item') .' '.
        t("For more info, see 'type' info at !link", array('!link' => l('drupal.org', 'http://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_menu/7'))),
      '#type' => 'select',
      '#default_value' => isset($settings['architecture']['menu_link']) ? $settings['architecture']['menu_link'] : 'MENU_CALLBACK',
      '#options' => drupal_map_assoc(array_keys($menus)) + array(
        MENU_CALLBACK => t('No menu item'),
        MENU_LOCAL_TASK => 'MENU_LOCAL_TASK',
        MENU_DEFAULT_LOCAL_TASK => 'MENU_DEFAULT_LOCAL_TASK',
        MENU_LOCAL_ACTION => 'MENU_LOCAL_ACTION'
      ),
      '#element_validate' => array(),
      '#weight' => 2
    ),
    'access' =>array(
      '#title' => t('Access control'),
      '#description' => t("In addition to currency access control, and block access control, access to this form can be restricted."),
      '#type' => 'select',
      '#default_value' => isset($settings['architecture']['access']) ? $settings['architecture']['access'] : 'user_chooser_segment_perms:transact',
      '#options' => module_invoke_all('uchoo_segments'),
      '#element_validate' => array(),
      '#weight' => 3
    ),
    'use_block' =>  array(
      '#title' => t('Make block available'),
      '#description' => t('The block is named according the path.') .' '.
        t('Remember that blocks have their own caching and access control.'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['architecture']['use_block']) ? $settings['architecture']['use_block'] : '',
      '#element_validate' => array(),
      '#weight' => 4
    )
  );

  $form['perspective'] = array(
    '#title' => t('perspective'),
    '#description' => t('Whether this is completed by a 3rd party or a participant.'),
    '#type' => 'radios',
    '#options' => $perspectives,
    '#default_value' => isset($settings['perspective']) ? $settings['perspective'] : 3,
    '#weight' => 5,
    //this ajax only works once then the form #id increments to ctools-export-ui-edit-item-form--2
    '#ajax' => array(
      'callback' => 'mcapi_forms_ajax_settings',
      'wrapper' => 'form-wrapper'
    ),
  );

  //following section of the form allows the admin to handle the individual fields of the transaction form.
  //the fields are handled here one in each tab, each field having some shared settings and some specific ones.
  $form['mcapi_form_fields'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 6,
  );
  //the participants of the transaction are either payer/payee or 1stperson/secondperson. This is called the 'perspective' of the form.
  $perspective = isset($form_state['values']['perspective']) ? $form_state['values']['perspective'] : $form['perspective']['#default_value'] ;

  //load the transaction form so we can re-use the widgets
  $array = array();
  $transaction_widgets = transaction_form($array, $form_state);
  //adjust the empty worth transaction widget because in these forms empty is blank not zero
  foreach ($transaction_widgets['worth'][LANGUAGE_NONE]['#default_value'] as $currcode => $quantity) {
    $transaction_widgets['worth'][LANGUAGE_NONE]['#default_value'][$currcode] = $quantity == '0' ? '' : $quantity;
  }
  foreach (element_children($transaction_widgets) as $fieldname) {
    $form[$fieldname] = (array)@$form[$fieldname];//this is good for new forms with values
    $settings[$fieldname] = (array)@$settings[$fieldname];//in case the form elements are controlled elsewhere and not preset here
    switch ($fieldname) {
      case 'type':
      case 'state':
      case 'creator':
      case 'buttons':
        break;
      case 'payer':
        if ($perspective == 1) {
          $transaction_widgets[$fieldname]['#title'] = t('Other user');
          //convert the payer field to a seconderson field
          if (!isset($settings['secondperson'])) $settings['secondperson'] = array();
          if (!isset($settings['direction'])) $settings['direction'] = array();
          $form['secondperson'] = _mcapi_user_chooser_config_field($settings['secondperson']) + _mcapi_form_config_fieldset($settings['secondperson'], $transaction_widgets['payer'], 'secondperson');
          $preset = array(
            '#title' => t('Direction'),
            '#description' => t("Either 'incoming' or 'outgoing' relative to the logged in user"),
            '#type' => isset($settings['direction']['widget']) ? $settings['direction']['widget'] : 'radios',
            '#options' => array(
              '' => t('Neither'),
              'incoming' => isset($settings['direction']['incoming']) ? $settings['direction']['incoming'] : t('Incoming'),
              'outgoing' => isset($settings['direction']['outgoing']) ? $settings['direction']['outgoing'] : t('Outgoing'),
            ),
            '#default_value' => '',
            '#required' => FALSE
          );
          $form['direction'] = _mcapi_form_config_fieldset($settings['direction'], $preset, 'direction')
            +_mcapi_direction_config_fields((array)$settings['direction']);
          break;
        }
        //then run on
      case 'payee':
        if ($perspective == 3) {
          $form[$fieldname] = _mcapi_user_chooser_config_field((array)$settings[$fieldname]) + _mcapi_form_config_fieldset($settings[$fieldname], $transaction_widgets[$fieldname], $fieldname);
          //$form[$fieldname] += ;
          $form[$fieldname]['preset']['#description'] = t('This field does not respond to changes in the above field.');
        }
        break;

      default: //this covers all fieldAPI fields, including 'worth'
        $instance = field_read_instance('transaction', $fieldname, 'transaction');
        $default_items = &$settings[$fieldname];
        $empty_field = $transaction_widgets[$fieldname][LANGUAGE_NONE];
        $form[$fieldname] = _mcapi_form_config_fieldset($default_items, $empty_field, $fieldname);
    }
  }
  $form['worth']['preset']['#preset'] = TRUE;
  $currencies = currencies_load();
  if (count($currencies) > 1) {
    $help = array(
      t("Use '0' to include a currency as an option on the form."),
      t('Leave blank to exclude the currency.'),
      t('Numeric values are also valid')
    );
    $form['worth']['#description'] = implode(' ', $help);
  }
  $form['worth']['preset']['#element_validate'] = array('mcapi_form_validate_worth_preset');
  //we need to know the view modes of the transaction entity, to choose for the preview page
  $entity = entity_get_info('transaction');
  foreach ($entity['view modes'] as $name => $mode) {
    if (in_array($name, array('search_index', 'search_result', 'token'))) continue;
    $view_modes[$name] = $mode['label'];
  }
  $form['experience'] = array(
    '#title' => t('User experience'),
    '#description' => t('Form & confirmation form appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#type' => 'fieldset',
    '#weight' => 15,
    'title' => array(
      '#title' => t('Title'),
      '#description' => t('For use in block title and menu links.') .' '. t('Leave blank to jump to the confirmation stage.'),
      '#type' => 'textfield',
      '#default_value' => @$settings['experience']['title'],
      '#weight' => 1,
      '#required' => TRUE
    ),
    'template' => array(
      '#title' => t('Main form'),
      '#description' => t('Use the tokens with HTML & css to design your payment form. Linebreaks will be replaced automatically.'),
      '#type' => 'textarea',
      '#rows' => 6,
      '#default_value' => @$settings['experience']['template'],
      '#weight' => 2,
      '#required' => TRUE
    ),
    'token_tree1' => array(
      '#theme' => 'token_tree',
      '#token_types' => array('mcapiform', 'user'),
      '#global_types' => FALSE,
      '#weight' => 3,
    ),
    'sure_title' => array(
      '#title' => t('Confirmation page title'),
      '#type' => 'textfield',
      '#default_value' => @$settings['experience']['sure_title'],
      '#weight' => 4,
    ),
    'sure_format' => array(
      '#title' => t('Confirmation preview mode'),
      '#type' => 'radios',
      '#options' =>  $view_modes + array(
        'token' => t('Tokenised sentence'),
      ),
      '#default_value' => isset($settings['experience']['sure_format']) ? $settings['experience']['sure_format'] : 'certificate',
      '#weight' => 5,
    ),
    'sure_template' => array(
      '#title' => t('use tokens here'),
      '#description' => t('Use the tokens with HTML & css to design your payment form. Linebreaks will be replaced automatically.'),
      '#type' => 'textarea',
      '#rows' => 6,
      '#default_value' => @$settings['experience']['sure_template'] ? $settings['experience']['sure_template'] : variable_get('mcapi_sentence', mcapi_sentence_default()),
      '#states' => array(
        'visible' => array(
          ':input[name="experience[sure_format]"]' => array('value' => 'token'),
        ),
        'required' => array(
          ':input[name="experience[sure_format]"]' => array('value' => 'token'),
        ),
      ),
      '#weight' => 6,
    )
  );
  $form['submission'] = array(
    '#title' => t('Form submission'),
    '#description' => t("Settings for @field", array('@field' => t('Submit'))),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => empty($settings),
    'button' => array(
      '#title' => t('Save button label'),
      '#description' => t("The text to appear on the 'save' button, or the absolute url of an image."),
      '#type' => 'textfield',
      '#default_value' => isset($settings['submission']['button']) ? $settings['submission']['button'] : 'Submit',
      '#required' => TRUE,
    ),
    'redirect' => array(
      '#title' => t('Destination'),
      '#description' => t('Goes to the new transaction certificate by default.'),
      '#type' => 'textfield',
      '#default_value' => isset($settings['submission']['redirect']) ? $settings['submission']['redirect'] : '',
      '#element_validate' => array('mcapi_path_validate'),
      '#weight' => 1,
    ),
    'feedback' => array(
      '#title' => t('Confirmation message'),
      '#description' => t('Contents of the message box.'),
      '#type' => 'textfield',
      '#default_value' => isset($settings['submission']['feedback']) ? $settings['submission']['feedback'] : '',
      '#weight' => 1,
    ),
    '#weight' => 20,
  );
  if (module_exists('i18n_string')) {//put some helpful links in
    $path = 'admin/accounting/forms/list/'. $settings['info']['name'] .'/translate/';
    $note = t('Put the default language here, then !link.',
      array('!link' => l(t('translate'), $path))
    ) .' ';
    $form['experience']['title']['#description'] = $note . $form['experience']['title']['#description'];
    $form['experience']['template']['#description'] = $note . $form['experience']['template']['#description'];
    $form['experience']['sure_title']['#description'] = $note . @$form['experience']['sure_title']['#description'];
    $form['submission']['button']['#description'] = $note . $form['submission']['button']['#description'];
    $form['submission']['feedback']['#description'] = $note . $form['submission']['feedback']['#description'];
    if ($perspective == 1) {
      $form['direction']['#description'] = $note . $form['direction']['#description'];
    }
  }
}

/*
 * make a fieldset for each of the fields on the transaction form
 * Each fieldset will include a field to set a default value
 * $mcapi_form_field - the saved section of the mcapi form
 * $empty_element - an empty widget to put the #default_value in
 * $key - the fieldname
 */
function _mcapi_form_config_fieldset($field_defaults, array $empty_element, $fieldname) {
  static $w;
  $preset = @$field_defaults['preset'];
  mcapi_unset_recursive($empty_element, '#required');
  mcapi_unset_recursive($empty_element, '#element_validate');
  $element = array(
    '#title' => $empty_element['#title'],
    '#type' => 'fieldset',
    '#group' => 'mcapi_form_fields',
    '#weight' => $w++,
    'preset' => array(
      '#title' => t('Default value'),
    ) + $empty_element,
  );
  //set the defaults from the saved object.
  //Need to do it differently according to whether this is a fieldAPI field or one of the basic transaction properties
  if (isset($empty_element['#entity'])) {//this is for fieldAPI worth checkboxes and radio buttons
    if (isset($preset)) {
      if ($fieldname == 'worth' && count($preset)) {
        $currencies = currencies_load();
        foreach ($preset as $item) {
          if (isset($currencies[$item['currcode']])) {//in case it has been disabled since.
            $element['preset']['#default_value'][$item['currcode']] = $item['quantity'];
          }
        }
      }
      else {
        $element['preset']['#default_value'] = $preset;
      }
    }
  }
  elseif (element_children($empty_element)) {//this is a normal fieldAPI field - not checkboxes or radios
    if(is_array($preset)) {
      foreach (element_children($empty_element) as $delta) {
        foreach (element_children($empty_element[$delta]) as $col) {
          $val = isset($preset[LANGUAGE_NONE][$delta][$col]) ? $preset[LANGUAGE_NONE][$delta][$col] : $preset[$delta][$col];
          if (strlen($val)) {
            $element['preset'][$delta][$col]['#default_value'] = $val;
          }
        }
      }
    }
  }
  else {//this is a transaction entity property and is required
    $element['#description'] = t("This field is required");
    $element['preset']['#default_value'] = $preset;
  }
  $element['strip'] = array(
    '#title' => t('Strip theme container'),
    '#description' => t("Disable the outer container, so it doesn't render the field #title and #description but gives you fuller control in the template below."),
    '#type' => 'checkbox',
    '#default_value' => @$field_defaults['strip'],
    '#weight' => 10
  );
  return $element;
}

function mcapi_forms_ajax_settings($form, &$form_state) {
  return $form;
}

/*
 * form validation callback for edit_mcapi_form
 */
function mcapi_forms_template_validate($form, &$form_state) {
  $values = &$form_state['values'];
  //check that the critical transaction fields are either present in the template OR preset
  $required_fields = array('worth');
  $required_fields += $values['perspective'] == 1 ? array('secondperson', 'direction') : array('payer', 'payee');
  foreach ($required_fields as $fieldname) {
    if (empty($values[$fieldname]['preset']) && strpos($values['experience']['template'], "[mcapiform:$fieldname]") === FALSE) {
      form_set_error('template', t('Either preset @tokenname, or include that token in the template.',
        array('@tokenname' => "[mcapiform:$fieldname]")
      ));
    }
  }
}

function mcapi_forms_exists($machine_name, $element, &$form_state) {
  $forms = mcapi_forms_load();
  return isset($forms['machine_name']);
}


/*
 * validate callback for edit form,
 * declared in the ctools plugin $this->plugin['form']['submit'] in mcapi_forms_ui.inc
 * the $form_state[item] is the object to be saved, derived from $form_state[values]
 */
function mcapi_edit_mcapi_form_validate($form, &$form_state) {
  $values = &$form_state['values'];
  //need to check that all required fields exist in the form.
  if (!empty($values['experience']['template'])) {
    foreach ($values as $name => $prop) {
      if (!empty($prop['required']) && empty($prop['preset'])) {
        if (strpos($values['experience']['template'], $name) == FALSE) {
          form_error(
            $form['experience']['template'],
            t("Required field '@name' must be used in the form template", array('@name' => $name))
          );
        }
      }
    }
  }
  if (variable_get('mcapi_mixed_transactions', TRUE) && empty($values['worth']['preset'])) {
    form_error($form['worth']['preset'], t("Use '0' in the worth field"));
  }
}

/*
 * ctools export_ui plugin callback:
 * The submit handler is only responsible for moving data from
 * $form_state['values'] to $form_state['item'].
 */
function mcapi_edit_mcapi_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  unset($form_state['values']['mcapi_form_fields']);
  //awkward, but on the one hand ctools assumes the 'name' is a top level element
  // and elsewhere it is expected to be in 'info'
  $form_state['values']['name'] = $form_state['values']['info']['name'];

  //save the strings for the i18n
  if (function_exists('i18n_string_object_update')) {
    //we don't need to pass the whole object because it isn't saved
    //mcapi_forms_i18n_object_info is set to expect this
    $strings = _mcapi_i18n_object($form_state['values']);
    i18n_string_object_update('payform_experience', $strings);
  }
  $form_state['item']->data = $form_state['values'];
}

/*
 * element validate callback for worth field with multiple cardinality
 * Ensure that null values are saved as NULL and not zero
 */
function mcapi_form_validate_worth_preset(&$element, &$form_state) {
  //if any of the currencies has been preset with a value, including 0 remove the empty ones before saving
  $items = &$form_state['values']['worth']['preset'];
  foreach ($items as $delta => $item) {
    if ($item['main_quant'] == '' && empty($item['div_quant'])) {
      //that means it won't be saved and will come in as blank next time
      unset($items[$delta]);
    }
  }
  //validate that not more than one currency is preset above zero
  // && variable_get('mcapi_mixed_transactions', TRUE)

  form_set_value($element, $items, $form_state);
}

function _mcapi_user_chooser_config_field(array $default) {
  $default += array(
    'user_chooser_config' => 'user_chooser_segment_perms:transact',
    'preset' => ''
  );
  $fields['user_chooser_config'] = array(
    '#title' => t('Users to choose from'),
    '#type' => 'select',
    '#options' => module_invoke_all('uchoo_segments'),
    '#default_value' => $default['user_chooser_config'],
    '#required' => TRUE,
  );
  $params = explode(':', $default['user_chooser_config']);

  $fields['preset'] = array(
    '#title' => t('Preset field to'),
    '#description' => t('Configure this widget more at !link',
       array('!link' => l('admin/config/people/user_chooser', 'admin/config/people/user_chooser', array('attributes'=>array('target'=> '_blank')))
       )),
    '#type' => 'user_chooser_few',
    '#callback' => array_shift($params),
    '#args' => $params,
    '#default_value' => $default['preset'],
    '#required' => FALSE,
    '#multiple' => FALSE
  );
  return $fields;
}


function _mcapi_direction_config_fields($defaults) {
  $defaults += array(
    'incoming' => t('Request'),
    'outgoing' => t('Payment'),
    'widget' => 'radios'
  );

  return array(
    'widget' => array(
      '#title' => t('Widget'),
      '#type' => 'radios',
      '#options' => array(
        'select' => t('Dropdown select box'),
        'radios' => t('Radio buttons')
      ),
      '#default_value' => $defaults['widget'],
      '#weight' => 1
    ),
    'incoming' => array(
      '#title' => t("@label option label", array('@label' => t('Incoming'))),
      '#type' => 'textfield',
      '#default_value' => $defaults['incoming'],
      '#weight' => 2
    ),
    'outgoing' => array(
      '#title' => t("@label option label",  array('@label' => t('Outgoing'))),
      '#type' => 'textfield',
      '#default_value' => $defaults['outgoing'],
      '#weight' => 3
    )
  );
  return $element;
}

function mcapi_element_validate_path_duplicate(&$element, &$form_state) {
  //strip the leading slash just to be sure
  if (substr($form_state['values']['architecture']['path'], 0, 1) == '/') {
    $element['#value'] = substr($form_state['values']['architecture']['path'], 1);
    form_set_value($element, $element['#value'], $form_state);
  }
  if ($element['#value'] != $element['#default_value']) {
    if (db_query("SELECT TRUE FROM {menu_router} WHERE path = :path", array(':path' => $element['#value']))->fetchField()) {
      form_error($element, t('Path is already in use'));
    }
  }


}
