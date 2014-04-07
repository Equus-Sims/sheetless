<?php


/*
 * implements hook_rules_data_info
 * defines types for actions parameters and for event variables
 */
function mcapi_rules_data_info() {
  //rules already knows about the transaction entity and about the worth field
}

/*
 * implements hook_rules_action_info
 */
function mcapi_rules_action_info() {
  //I'd really really like a way for rules to add dependent transactions to the cluster during hook_transaction_cluster_alter
  //however rules doesn't expect to alter its own parameters as far as i can see
  //So this would involve:
  //creating a new rules datatype, for the transaction cluster
  //pass it by reference by calling $event->executeByArgs($args) directly during hook_transaction_cluster_alter
  //
  //A half way measure would involve inserting the transaction after it was created, but using the same serial number.
  //this would mean the transaction wouldn't show up on the 'are you sure' page but would appear afterwards in the cluster.
}


class TransactionRulesController extends EntityDefaultRulesController{

  //the variables for each event determine what data selectors are available in the conditions
  public function eventInfo() {
    $info = $this->info;
    $type = $this->type;

    $label = $info['label'];
    $triggers = mcapi_trigger_info();
    //we'll use the triggers declared
    foreach ($triggers['mcapi'] as $trigger_callback => $trigger) {
      //There were just too many obstacles to making 'transaction_cluster_alter' work with rules
      if ($trigger_callback == 'transaction_cluster_alter') continue;

      switch($trigger_callback) {
        case 'transaction_update':
          //state is the same as the other events except it expects a second argument
          $variables = array(
            'transaction' => array(
              'type' => 'transaction',
              'label' => t('Transaction')
            ),
            'state' => array(
              'type' => 'integer',
              'label' => t('Transaction state')
            )
          );
          break;
        case 'transaction_delete':
        case 'transaction_view':
        default://these include the transaction_operations
          $variables = array(
            'transaction' => array(
              'type' => 'transaction',
              'label' => t('Transaction')
            )
          );
      }
      //add this to every
      $items[$trigger_callback] = array(
        'module' => 'mcapi',
        'group' => t('Community accounting'),
        'label' => $trigger['label'],
        'variables' => $variables
      );
    }
    return $items;
  }
}


/**
 * UI for configuring worth field
 */
class RulesDataUIWorth extends RulesDataUI implements RulesDataDirectInputFormInterface {

  public static function getDefaultMode() {
    return 'input';
  }

  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $form = array(
      $name => array(
        '#title' => t('Worth at least'),
        '#type' => 'worth',
        '#default_value' => isset($settings[$name]) ? $settings[$name] : array(),
        '#all' => TRUE,
         //this is not config mode because it's not 'presetting the form its actually setting a value
         //it affects what happens to the zero and null values
        '#required' => TRUE,
      )
    );
    return $form;
  }

  public static function render($value) {
    //this is similar to mcapi_field_formatter_view, but just renders a single item
    //I don't know how to invoke this or test it
    $renderable = array(
      '#theme' => 'worth_item',
      '#currcode' => $value['currcode'],
      '#quantity' => $value['quantity']
    );
    return $renderable;
  }
}
