$(document).ready(function() {
    
    $('#p1').click(function() {
      $.post('/grid/26');
    });
    $('#r1').click(function() {
      $.post('/grid/27');
    });
    $('#i1').click(function() {
      $.post('/grid/28');
    });

  });
  
