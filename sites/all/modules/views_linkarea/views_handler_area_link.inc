<?php

class views_handler_area_link extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['html'] = array('default' => 0);
    $options['path'] = array('default' => '');
    $options['querystring'] = array('default' => '');
    $options['anchor'] = array('default' => '');
    $options['class'] = array('default' => '');
    $options['return'] = array('default' => FALSE);
    $options['title'] = array('default' => '');
    $options['rel'] = array('default' => '');
    $options['target'] = array('default' => '');
    $options['tokenize'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#default_value' => $this->options['text'],
      '#description' => t('The text of the link'),
    );
    $form['html'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display link text as HTML'),
      '#default_value' => $this->options['html'],
      '#description' => t('Show link text as HTML instead of plain text.'),
    );
    $form['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Link path'),
      '#default_value' => $this->options['path'],
      '#description' => t('The Drupal path or full URL to which to link'),
    );
    $form['querystring'] = array(
      '#type' => 'textfield',
      '#title' => t('Link querystring'),
      '#default_value' => $this->options['querystring'],
      '#description' => t('The query parameters that follow the full path'),
    );
    $form['return'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set destination as the current page'),
      '#default_value' => $this->options['return'],
      '#description' => t('If the link leads to a form, this will redirect the user to the view when the form is submitted. This will override any destination set in the querystring option above.'),
    );
    $form['anchor'] = array(
      '#type' => 'textfield',
      '#title' => t('Link anchor'),
      '#default_value' => $this->options['anchor'],
      '#description' => t('The anchor data that follows the full path and query parameters'),
    );

    $form['attributes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attributes'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['class'] = array(
      '#type' => 'textfield',
      '#title' => t('Link CSS class'),
      '#default_value' => $this->options['class'],
      '#description' => t('A custom CSS class to add to the link'),
      '#fieldset' => 'attributes',
    );

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Link title'),
      '#description' => t('Set the title attribute of the link'),
      '#default_value' => $this->options['title'],
      '#fieldset' => 'attributes',
    );

    $form['rel'] = array(
      '#type' => 'textfield',
      '#title' => t('Link rel'),
      '#description' => t('Set the rel attribute of the link'),
      '#default_value' => $this->options['rel'],
      '#fieldset' => 'attributes',
    );

    $target_options = array(
      ''        => t('None'),
      '_blank'  => t('New window (_blank)'),
      '_parent' => '_parent',
      '_self'   => '_self',
      '_top'    => '_top',
    );

    $form['target'] = array(
      '#type' => 'select',
      '#title' => t('Link target'),
      '#description' => t('Set the target attribute of the link.'),
      '#options' => $target_options,
      '#default_value' => $this->options['target'],
      '#fieldset' => 'attributes',
    );

    $tokenize_id = drupal_html_id('tokenize_checkbox');
    $form['tokenize'] = array(
      '#id' => $tokenize_id,
      '#type' => 'checkbox',
      '#title' => t('Use replacement tokens from the first row'),
      '#default_value' => $this->options['tokenize'],
    );

    // Get a list of the available fields and arguments for token replacement.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {

      $options[t('Fields')]["[$field]"] = $handler->ui_name();
    }

    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    $output = t('<p>You must add some additional fields to this display before using this field. These fields may be marked as <em>Exclude from display</em> if you prefer. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');

    if (!empty($options)) {

      $output = '';

      foreach ($options as $type => $values) {

        if (empty($values)) {

          continue;
        }

        $vars = array('title' => $type);
        foreach ($values as $key => $label) {

          $vars['items'][] = $key . ' == ' . $label;
        }

        $output .= theme('item_list', $vars);
      }
    }

    $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#description' => '<p>' . t('The following tokens are available. If you would like to have the characters \'[\' and \']\' please use the html entity codes \'%5B\' or \'%5D\' or they will get replaced with empty space.') . '</p>',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => $output,
      '#dependency' => array(
        $tokenize_id => array(1),
      ),
    );
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
  }

  function render($empty = FALSE) {

    if ($empty && empty($this->options['empty'])) {

      return '';
    }

    $tokens = $this->get_render_tokens();

    // Replace any tokens in the text.
    $text = strtr($this->options['text'], $tokens);

    // Replace any tokens in the path.
    $path = strtr($this->options['path'], $tokens);

    // Check that the user has access to the menu router item, but only if the
    // path is for a valid menu router item, so that external URLs or paths not
    // handled by Drupal's menu router are always permitted.
    $router_item = menu_get_item($path);
    if ($router_item && !$router_item['access']) {
      return '';
    }

    // Where we store tokenized values.
    $link_properties = array();
    // Options that will go into the attributes array for url().
    $attribute_keys = array('title', 'target', 'rel', 'class');
    // Other options that we need to put into link properties.
    $option_keys = array_merge(array('anchor', 'text', 'path'), $attribute_keys);

    if (!empty($this->options['querystring'])) {
      // This is an ugly way to do it, but Drupal 7 now takes an array for
      // query instead of a string.  That's good, but makes our string field
      // not work.  This should get switched to a multi-value interface of
      // some kind instead of ugly string parsing. @todo
      $querystring = strtr($this->options['querystring'], $tokens);
      $link_properties['query'] = drupal_get_query_array($querystring);
    }

    if (!empty($this->options['return'])) {
      $destination = drupal_get_destination();
      $link_properties['query']['destination'] = $destination['destination'];
    }

    // Grab all of our options and tokenize them if necessary.
    foreach ($option_keys as $key) {

      if (empty($this->options[$key])) {

        continue;
      }

      if ('anchor' == $key) {
        $link_properties['fragment'] = $this->options[$key];
      }
      else {
        $link_properties[$key] = $this->options[$key];
      }

      // Apply the argument substitutions.
      if (!empty($tokens)) {
        if ('anchor' == $key) {
          $link_properties['fragment'] = str_replace(array_keys($tokens), array_values($tokens), $link_properties['fragment']);
        } else {
          $link_properties[$key] = str_replace(array_keys($tokens), array_values($tokens), $link_properties[$key]);
        }
      }

      // Apply the more advanced tokenization.
      if ($this->options['tokenize']) {
        if ('anchor' == $key) {
          $link_properties['fragment'] = $this->view->style_plugin->tokenize_value($link_properties['fragment'], 0);
        } else {
          $link_properties[$key] = $this->view->style_plugin->tokenize_value($link_properties[$key], 0);
        }
      }
    }

    if (empty($link_properties['attributes'])) {

      $link_properties['attributes'] = array();
    }

    // Move our attributes into an attribute array for ease of use with url().
    foreach ($attribute_keys as $key) {

      if (!empty($link_properties[$key])) {

        $link_properties['attributes'][$key] = $link_properties[$key];

        if ('class' === $key) {

          $link_properties['attributes'][$key] = explode(' ', $link_properties['attributes'][$key]);
        }
      }
    }

    if ($this->options['html']) {
      $text = filter_xss_admin($text);
      $link_properties['html'] = TRUE;
    }
    else {
      // Make sure all HTML entities are decoded before passing to l().
      while (decode_entities($text) != $text) {

        $text = decode_entities($text);
      }
    }

    return l($text, $path, $link_properties);
  }

  /**
   * Gets appropriate views replacement tokens for this handler.
   *
   * This code is largely based on views_handler_field's token rendering, but
   * we only care about arguments.  The render() method's link generation
   * handles XSS for us.
   */
  function get_render_tokens() {
    $tokens = array();
    if (!empty($this->view->build_info['substitutions'])) {
      $tokens = $this->view->build_info['substitutions'];
    }
    $count = 0;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $token = '%' . ++$count;
      if (!isset($tokens[$token])) {
        $tokens[$token] = '';
      }

      $tokens['!' . $count] = isset($this->view->args[$count - 1]) ? check_plain($this->view->args[$count - 1]) : '';
    }
    return $tokens;
  }
}
