var navbar = document.getElementsByClassName('navbarall');

window.onscroll = function() {
  // pageYOffset or scrollY
  if(window.innerWidth < 400 && window.pageYOffset > 45){
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = '#a2a2a2';
  } else if(window.innerWidth < 600 && window.pageYOffset > 150){
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = '#a2a2a2';
  } else if(window.innerWidth < 800 && window.pageYOffset > 160){
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = '#a2a2a2';
  } else if(window.innerWidth < 1200 && window.pageYOffset > 160){
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = '#a2a2a2';
  } else if (window.pageYOffset > 310) {
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = '#a2a2a2';
  } else {
    document.getElementsByClassName('navbarall')[0].style.backgroundColor = 'transparent';
  }
}
