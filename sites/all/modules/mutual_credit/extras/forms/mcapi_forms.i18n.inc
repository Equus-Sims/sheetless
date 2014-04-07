<?php


/**
 * Implements hook_i18n_string_info().
 */
function mcapi_forms_i18n_string_info() {
  $groups['payforms'] = array(
    'title' => t('Designed forms'),
    'description' => t('Templates for payment forms'),
    'format' => TRUE,
    'list' => TRUE,
  );
  return $groups;
}


/**
 * Implements hook_i18n_object_info().
 */
function mcapi_forms_i18n_object_info() {
  $info['payform_experience'] = array(
    // Generic object title.
    'title' => t('Community accounting designed forms'),
    'class' => 'i18nmcapiform',
    // The object (or array) key field.
    'key' => 'name',
    // Placeholders for automatic paths.
    'placeholders' => array(
      '%ctools_export_ui' => 'name',
    ),
    // To produce edit links automatically.
    'edit path' => 'admin/accounting/workflow/forms/list/%ctools_export_ui/edit',
    // Auto-generate translate tab.
    'translate tab' => 'admin/accounting/workflow/forms/list/%ctools_export_ui/translate',
    // Properties for string translation.
    'string translation' => array(
      // Text group that will handle this object's strings.
      'textgroup' => 'payforms',
      // Object type property for building up the translation name thus payforms:formname:experience:property
      'type' => 'experience',
      // Table where the object is stored, to automate string lists
      'table' => 'mcapi_forms',
      // Translatable properties of these objects.
      'properties' => array(
        'title' => t('Page title'),
        'template' => array(
          'title' => t('Template'),
          'format' => 'templates_format',
        ),
        'sure_title' => t('Sure title'),
        'button' => t('Save button label'),
        'feedback' => t('Confirmation message'),
        'incoming' => t('Incoming direction'),
        'outgoing' => t('Outgoing direction'),
      ),
      // Path to translate strings to every language.
      'translate path' => 'admin/accounting/forms/list/%ctools_export_ui/translate/%i18n_language',
    )
  );
  return $info;
}

class i18nmcapiform extends i18n_string_object_wrapper {

  /**
   * Get translatable properties
   */
  protected function build_properties() {
    $strings = parent::build_properties();
    $mcapiform_experience = (array)$this->object;
    $textgroup = $this->get_textgroup();
    foreach ($strings['payforms']['experience'][$mcapiform_experience['name']] as $fieldname => $item) {
      $strings[$textgroup]['experience'][$mcapiform_experience['name']][$fieldname] = array(
        'title' => $item['title'],
        'string' => @$mcapiform_experience[$fieldname]
      );
    }
    return $strings;
  }
}
