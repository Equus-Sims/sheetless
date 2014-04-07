<?php
$plugin = array(
  // The name of the table as found in the schema in hook_install. This
  // must be an exportable type with the 'export' section defined.
  'schema' => 'mcapi_forms',

  // The access permission to use. If not provided it will default to
  // 'administer site configuration'
  'access' => 'design transaction forms',

  // You can actually define large chunks of the menu system here. Nothing
  // is required here. If you leave out the values, the prefix will default
  // to admin/structure and the item will default to the plugin name.
  'menu' => array(
    'menu prefix' => 'admin/accounting/workflow',
    'menu item' => 'forms',
    // Title of the top level menu. Note this should not be translated,
    // as the menu system will translate it.
    'menu title' => 'Form designer',
    // Description of the top level menu, which is usually needed for
    // menu items in an administration list. Will be translated
    // by the menu system.
    'menu description' => 'Gives you fine-grained control of the all important transaction form. Control the wording, the layout, the presets, the access and other aspects of the drupal architecture.',
  ),

  // These are required to provide proper strings for referring to the
  // actual type of exportable. "proper" means it will appear at the
  // beginning of a sentence.
  'title singular' => t('transaction form'),
  'title singular proper' => t('Transaction form'),
  'title plural' => t('transaction forms'),
  'title plural proper' => t('Transaction forms'),

  // This will provide you with a form for editing the properties on your
  // exportable, with validate and submit handler.
  //
  // The item being edited will be in $form_state['item'].
  //
  // The submit handler is only responsible for moving data from
  // $form_state['values'] to $form_state['item'].
  //
  // All callbacks will accept &$form and &$form_state as arguments.
  'form' => array(
    'settings' => 'mcapi_edit_mcapi_form',
    'validate' => 'mcapi_edit_mcapi_form_validate',
    'submit'   => 'mcapi_edit_mcapi_form_submit',
  ),
  'strings' => array(
    'confirmation' => array(
      'revert' => array(
        'information' => t('This action will permanently remove any customizations made to this form.'),
        'success' => t('The form has been reverted.'),
      ),
      'delete' => array(
        'information' => t('This action will permanently remove the form from your database.'),
        'success' => t('The form has been deleted.'),
      ),
    ),
  ),
  'handler' => 'mcapi_forms_ui',
);
