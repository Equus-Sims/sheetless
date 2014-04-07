<?php

/*
 * implements entity hook_entity_property_info for the transaction entity
 * required field makes it appear in the rules new entity config form
 * TODO: try enabling 'entity views field' for each of these and see if it is as good as in transactionDefaultViewsController
 */
function mcapi_entity_property_info() {
  $info['transaction']['properties']['serial'] = array(
    'label' => t("Transaction ID"),
    'type' => 'text',
    'description' => t("The unique serial number of the transaction."),
    'schema field' => 'serial',
  );
  $info['transaction']['properties']['transaction_type'] = array(
    'label' => t("Transaction type"),
    'type' => 'text',
    'options list' => 'mcapi_get_types',
    'schema field' => 'type',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  $info['transaction']['properties']['payer'] = array(
    'label' => t("Payer"),
    'description' => t("The user ID of the payer"),
    'type' => 'user',
    'schema field' => 'payer',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  $info['transaction']['properties']['payee'] = array(
    'label' => t("Payee"),
    'description' => t("The user ID of the payee"),
    'type' => 'user',
    'schema field' => 'payee',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  $info['transaction']['properties']['created'] = array(
    'label' => t("Transaction creation moment"),
    'description' => t("Stored as unixtime, displayed as a date"),
    'type' => 'date',
    'schema field' => 'created',
  );
  $info['transaction']['properties']['creator'] = array(
    'label' => t("User who created the transaction"),
    'type' => 'user',
    'schema field' => 'creator',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  $info['transaction']['properties']['state'] = array(
    'label' => t("State"),
    'type' => 'integer',
    'options list' => 'mcapi_get_states',
    'schema field' => 'state',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
    'entity token' => FALSE
  );
  return $info;
}


/*
 * implements hook_entity_views_field_handlers_alter
 */
function mcapi_entity_views_field_handlers_alter(&$field_handlers) {
  //this views handler isn't written yet coz I haven't worked out how the entity module integrates with views
  //$field_handlers['worth'] = 'entity_views_handler_field_mcapi_worth';
}
