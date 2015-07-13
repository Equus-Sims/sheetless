<?php
// $Id: views_linkarea.views.inc,v 1.1.4.1 2010/11/18 20:17:19 crell Exp $

/**
 * Implementation of hook_views_data()
 */
function views_linkarea_views_data() {
  $data['views']['link'] = array(
    'title' => t('Link'),
    'help' => t('Provide an internal or external link.'),
    'area' => array(
      'handler' => 'views_handler_area_link',
    ),
  );

  return $data;
}
