<?php

/**
 * @file
 * Command line utility for processing computed fields.
 */

/**
 * Implements hook_drush_command().
 */
function computed_field_tools_drush_command() {
  $items = array();

  $items['cft-recompute'] = array(
    'callback' => 'computed_field_tools_drush_recompute',
    'description' => dt('Re-compute computed field'),
    'arguments' => array(
      'field_name_to_recompute' => "computed_field name to process.",
    ),
    'options' => array(
      'limit' => 'Use "--limit=10" (Integer) to limit the number of entities to process.',
      'output' => 'Use "--output=0" (0/1) to disable any print outs while processing the fields.',
    ),
    'aliases' => array('cftr'),
  );

  return $items;
}

/**
 * Drush callback function to process computed fields.
 *
 * @param string $field_name_to_recompute
 *   String telling which field to process. If left empty, a choice is given.
 *
 * @option $limit
 *   Integer limiting the number of fields. Should only be used when debugging.
 * @option $output
 *   Integer 1/0 so that we can tell the process to keep quiet.
 *  (If no field_name is given. the choice is displayed though)
 */
function computed_field_tools_drush_recompute($field_name_to_recompute = '') {
  // Get options if any given.
  $limit = (int) drush_get_option('limit', 0);
  $verbose = drush_get_option('output', 1);

  $field_info_fields = field_info_fields();

  // Collect fields of type computed_field.
  $computed_fields_options = array();
  foreach ($field_info_fields as $field) {
    if ($field['type'] == 'computed' && $field['module'] == 'computed_field') {
      $computed_fields_options[$field['field_name']] = $field['field_name'];
    }
  }

  // Check that we have any computed fields.
  if (empty($computed_fields_options)) {
    if ($verbose) {
      drush_print(dt('No computed_field fields found.'));
    }
    return;
  }

  // Validate the inputted computed_field name, if any given.
  if (!empty($field_name_to_recompute) && !in_array($field_name_to_recompute, $computed_fields_options)) {
    if ($verbose) {
      drush_print(dt('No computed_field fields found with given name.'));
    }
    return;
  }

  // Get the field to process, if none is given as param.
  if (empty($field_name_to_recompute)) {
    $field_name_to_recompute = drush_choice($computed_fields_options, 'Select which field to process.');
  }

  if (empty($field_name_to_recompute)) {
    if ($verbose) {
      drush_print(dt('No computed_field chosen.'));
    }
    return;
  }

  // Collect which entities to process.
  $entities_by_type = $field_info_fields[$field_name_to_recompute]['bundles'];
  if (empty($entities_by_type)) {
    if ($verbose) {
      drush_print(dt('No entity types found for computed_field "!field_name".', array('!field_name' => $field_name_to_recompute)), 'error');
    }
    return;
  }

  if ($verbose) {
    drush_print(dt('Working on computed field: "!field_name"', array('!field_name' => $field_name_to_recompute)));
  }

  // Context is used to hold the status of each run. It has the same function as
  // when used through the batch api.
  $context = array();

  // Default entities to process on each run. By limiting the entities per run,
  // we simulate the batch routine as well as keeping the memory usage down.
  $entities_per_run = 30;

  // If a limit is given, the values is validated and only respected if
  // acceptable.
  $total_number_of_entities = (empty($limit) || !is_numeric($limit)) ? _computed_field_tools_get_total_batch_count($entities_by_type) : $limit;

  // Process the entities in batches.
  for ($i = 0; $i < $total_number_of_entities; $i += $entities_per_run) {
    // If any limit has been set, we make sure that we can't select more than
    // the given limit.
    $entities_per_run = ($entities_per_run > $total_number_of_entities) ? $total_number_of_entities : $entities_per_run;

    // Process the fields. Context takes care of the increment of entity id.
    _computed_field_tools_batch_recompute($entities_by_type, $field_name_to_recompute, $entities_per_run, $context);

    // Check if we are finished.
    if ($context['finished'] >= 1) {
      if ($verbose) {
        $duration = round($context['results']['end'] - $context['results']['start'], 2);
        $average_time_pr_thousand = round($duration * 1000 / $context['results']['total_id_count'], 2);
        drush_print(dt('Re-computed !entities_recomputed of !total_count fields in !duration seconds', array(
          '!entities_recomputed' => $context['results']['total_entities_touched'],
          '!total_count' => $context['results']['total_id_count'],
          '!duration' => $duration,
        )));
        drush_print(dt('It took an average of !average_time_pr_thousand seconds per 1000 fields to compute.', array('!average_time_pr_thousand' => $average_time_pr_thousand)));
      }
      break 1;
    }

    // Estimate the time. Just nice to know if you should get a cup of coffee.
    // (The time estimate in the first run will be a bit misleading as the
    // complete page request is not included in the estimate.)
    $time_left = t('Calculating...');
    if ($context['results']['total_entities_touched'] > $entities_per_run) {
      $time_spent = time() - $context['results']['start'];
      $time_left_in_seconds = round(($time_spent / $context['results']['total_entities_touched']) * ($context['results']['total_id_count'] - $context['results']['total_entities_touched']));
      $time_left = t('@minutes minutes @seconds seconds', array(
        '@minutes' => ($time_left_in_seconds - ($time_left_in_seconds % 60)),
        '@seconds' => ($time_left_in_seconds % 60),
      ));
    }

    if ($verbose) {
      drush_print(dt('Processed !entities of !entities_total entities. Estimated time left: !time_left. Memory peak: !memory_peakMB', array(
        '!entities' => $context['results']['total_entities_touched'],
        '!entities_total' => $context['results']['total_id_count'],
        '!time_left' => $time_left,
        '!memory_peak' => round(memory_get_peak_usage() / 1024 / 1024, 2),
      )));
    }
  }
}
