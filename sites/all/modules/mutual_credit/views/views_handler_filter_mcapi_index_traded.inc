<?php

class views_handler_filter_mcapi_index_traded extends views_handler_filter_boolean_operator {

  function query() {
    $this->ensure_my_table();
    $this->query->add_where(
      $this->options['group'],
      "$this->table_alias.uid",
      db_query("SELECT uid FROM users u WHERE uid NOT IN (SELECT uid1 FROM mcapi_index)")->fetchCol(),
      $this->value ? 'NOT IN' : 'IN'
    );
  }
  
}