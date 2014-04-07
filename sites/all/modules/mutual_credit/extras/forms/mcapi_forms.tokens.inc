<?php

/*
 * implements hook_tokens
 * $options is ver important because it tells us whether to render the form field or a text version of the value in it
 */
function mcapi_forms_tokens($type, $tokens, array $objects, array $options = array()) {
  $replacements = array();
  if ($type != 'mcapiform' || !isset($objects[$type])) return $replacements;
  $form = $objects[$type];
  $url_options = array('absolute' => TRUE);
  $sanitize = !empty($options['sanitize']);
  $language_code = NULL;

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  foreach ($tokens as $name => $original) {
    $replacements[$original] = drupal_render($form[$name]);
  }
  //tack the button on the end of the last replacement if it hasn't already been rendered as a token
  unset($form['buttons']['#theme_wrappers']);
  $replacements[$original] .= drupal_render($form['buttons']);

  //now, since we couldn't pass the form by reference,
  //we need to render the rest of the form elements, which not being in the template, are 'hidden'
  //and tack them onto the end of the last token - after the buttons
  foreach (element_children($form) as $name) {
    $element = &$form[$name];
    if (empty($element)) {//ideally this situation would be caught earlier.
      drupal_set_message(t("nonexistent field '@fieldname' in transaction form", array('@fieldname' => $name)));
      continue;
    }
    //skip any fields which have already been rendered, above
    if (!empty($element['#printed'])) {
      continue;
    }
    elseif (in_array($name, array('secondperson', 'direction', 'worth', 'payer', 'payee'))) {
      if ($name == 'direction' && !empty($element['#strip'])) {
        $element['#prefix'] = "<style>.form-item-direction{display:none;}</style>";
      }
      else {
        $element['#prefix'] = "<style>#edit-$name{display:none;}</style>";
      }
    }
    //hide any other fieldAPI elements
    elseif (array_key_exists('#type', $element) && $element['#type'] == 'container') {
      $element['#access'] = FALSE;
    }
    $replacements[$original] .= drupal_render($element);
  }

  return $replacements;
}

function mcapi_forms_token_info() {
  $types['mcapiform'] = array(
    'name' => t('Mcapi forms'),
    'description' => t('Tokens to put in stage 1 of the transaction forms'),
    'needs-data' => 'mcapiform',
  );

  $mcapiform = array(
    'payer' => array(
      'name' => t('3rd party: Payer name'),
      'description' => t("User whose account is credited"),
    ),
    'payee' => array(
      'name' => t('3rd party: Payee name'),
      'description' => t("User whose account is debited"),
    ),
    'direction' => array(
      'name' => t('1st party: Direction of transaction'),
      'description' => t("Relative to the logged in user"),
    ),
    'secondperson' => array(
      'name' => t('1st party: Second person'),
      'description' => t("The 'other' user in the transaction, not the logged in user"),
    ),
    'created' => array(
      'name' => t('Created date'),
      'description' => t("Changing the date of the transaction from now"),
    ),
  );

  foreach (field_info_instances('transaction', 'transaction') as $fieldname => $instance) {
    $mcapiform[$fieldname] = array(
      'name' => $instance['label'],
      'description' => isset($instance['description']) ? $instance['description'] : ''
    );
  }
  return array(
    'types' => $types,
    'tokens' => array('mcapiform' => $mcapiform),
  );
}


/*
 * nesting function to set values at the deepest level in the element
 */
function mcapi_element_children_value(&$element, $name, $value) {
  if (is_array($element)) {
    if ($fieldnames = element_children($element)) {
      foreach($fieldnames as $fieldname) {
        mcapi_element_children_value($element[$fieldname], $name, $value);
      }
    }
    else $element[$name] = $value;
  }
}
