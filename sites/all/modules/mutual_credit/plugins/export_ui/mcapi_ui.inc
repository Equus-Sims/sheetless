<?php
$plugin = array(
  // The name of the table as found in the schema in hook_install. This
  // must be an exportable type with the 'export' section defined.
  'schema' => 'mcapi_currencies',

  // The access permission to use. If not provided it will default to
  // 'administer site configuration'
  'access' => 'configure all currencies',

  // You can actually define large chunks of the menu system here. Nothing
  // is required here. If you leave out the values, the prefix will default
  // to admin/structure and the item will default to the plugin name.
  'menu' => array(
    'menu prefix' => 'admin/accounting',
    'menu item' => 'currencies',
    'menu title' => 'Currencies',
    'menu description' => 'Each currency implies a set of values and a community of users who economic fortunes are intertwined. Most communities will want only one currency, but if you are hosting many communities in one drupal instance, or if there is an appetite for experimentation, different currencies can be provided. User can access many potentially very different currencies. Mixed payments are also possible, but this is pushing the boundaries of the software.',
  ),

  // These are required to provide proper strings for referring to the
  // actual type of exportable. "proper" means it will appear at the
  // beginning of a sentence.
  'title singular' => t('currency'),
  'title singular proper' => t('Currencies'),
  'title plural' => t('currencies'),
  'title plural proper' => t('Currencies'),

  // The item being edited will be in $form_state['item'].
  //
  // The submit handler is only responsible for moving data from
  // $form_state['values'] to $form_state['item'].
  //
  // All callbacks will accept &$form and &$form_state as arguments.
  'form' => array(
    'settings' => 'mcapi_currency_form',
    'validate' => 'mcapi_currency_form_validate',
    'submit' => 'mcapi_currency_form_submit',
  ),
  'strings' => array(
    'confirmation' => array(
      'revert' => array(
        'information' => t('This action will permanently remove any customizations made to this currency.'),
        'success' => t('The currency has been reverted.'),
      ),
      'delete' => array(
        'information' => t('This action will permanently remove the currency from your database.'),
        'success' => t('The currency has been deleted.'),
      ),
    ),
  ),
  'handler' => 'mcapi_ui',
);

module_load_include('admin.inc', 'mcapi');