<?php
/**
 * @file
 * Drush integration for TinyPNG.
 */

/**
 * The TinyPNG library URI.
 */
define('TINIFY_PHP_VERSION', '0.9.1');
define('TINIFY_PHP_DOWNLOAD_URI', 'https://github.com/tinify/tinify-php/archive/' . TINIFY_PHP_VERSION . '.zip');

/**
 * Implements of hook_drush_command().
 */
function tinypng_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['tinypng-library'] = array(
    'callback' => 'drush_tinypng_tinify_download',
    'description' => dt('Download and install the Tinify PHP library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Tinify PHP library. If omitted Drush will use the default location.'),
    ),
  );

  return $items;
}

/**
 * Implements of hook_drush_help().
 */
function tinypng_drush_help($section) {
  switch ($section) {
    case 'drush:tinypng-library':
      return dt('Download and install the Tinify PHP library from https://github.com/tinify/tinify-php, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Tinify PHP Library.
 */
function drush_tinypng_tinify_download() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file(TINIFY_PHP_DOWNLOAD_URI, TINIFY_PHP_VERSION . '.zip')) {
    $filename = basename($filepath);
    $dirname =  basename($filepath, '.zip');

    // Remove any existing Tinify PHP library directory.
    if (is_dir($dirname) || is_dir(TINIFY_PHP_DIRNAME)) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir(TINIFY_PHP_DIRNAME, TRUE);
      drush_log(dt('A existing Tinify PHP library was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname);
    drush_move_dir(TINIFY_PHP_VERSION . '/' . TINIFY_PHP_DIRNAME . '-' . TINIFY_PHP_VERSION, TINIFY_PHP_DIRNAME, TRUE);
    drush_delete_dir(TINIFY_PHP_VERSION);
  }

  if (is_dir($dirname)) {
    drush_log(dt('Tinify PHP library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Tinify PHP library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
