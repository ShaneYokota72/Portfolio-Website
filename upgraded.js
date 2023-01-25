var navbar = document.getElementsByClassName('navbarall');

window.onscroll = function() {
  // pageYOffset or scrollY
  if (window.pageYOffset > 350) {
    //alert("active");
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = '#a2a2a2';
    // navbar.classList.add('scrolled')
  } else {
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = 'transparent';
    // navbar.classList.remove('scrolled')
  }
}
