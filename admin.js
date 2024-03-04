function showSection(sectionId) {
    // Hide all sections
    var sections = document.querySelectorAll('.content-section');
    sections.forEach(function(section) {
        section.style.display = 'none';
    });
    
    // Show the selected section
    var selectedSection = document.getElementById(sectionId);
    if (selectedSection) {
        selectedSection.style.display = 'block';
    }
}

// Default to show the first section
window.onload = function() {
    showSection('review-section');
};
