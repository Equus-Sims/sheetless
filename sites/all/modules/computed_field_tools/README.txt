README.txt
==========

The computed field tools module offers a way to re-compute the CCK computed
fields of existing nodes. It does so through the Batch API.

When using the Drupal module Computed Field (CCK) you sometimes make changes to
the logic behind the value in the computed field. If you wish to avoid re-saving
all nodes using the computing field, you can use this tool to re-compute all the
values again.

It is possible to choose which field (cross nodes) to re-compute and you can
also choose which node types you whish to re-compute.

When the batch is running it does not save the entire node again, but it only
saves the computed field.

Please note that when you re-compute the nodes, the node is fetched through
node_load() which means that the format of some values might defer from when you
submit the node through the node edit form. $node->taxonomy does this.
