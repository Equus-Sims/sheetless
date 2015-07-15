16/07/2012, 21:00


  Upload default avatar

Upload default avatar is simple module that adds ability to upload
default user picture into account settings page. Since you upload it,
you may use it with image styles in normal way into your custom code
snippets or user submodule to display user picture block.


    Why?

Because by core design you can't upload default user picture. Also by
core design, you can't use |theme_image_style| for files which is not in
the files directory. That means you can't use |theme_image_style| in
your code for your custom default user picture. So you have only two ways:

 1. Use this module to upload default user picture.
 2. Upload it to files dir by FTP ('cause sometimes default avatar is
    needed by website design).


    In additional

In additional, this module has submodule "User's avatar" block, which
adds the block with user's picture. Into this block settings you'll need
to assign image style to display avatar and set optional class for
avatar wrapper.



------------------------------------------------------------------------
© EllECTRONC, 2012 <http://drupalblog.ru/>

