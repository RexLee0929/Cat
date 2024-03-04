document.getElementById('loginForm').addEventListener('submit', function(event) {
    event.preventDefault(); // 阻止表单的默认提交行为

    const formData = {
        username: document.querySelector('input[name="username"]').value,
        password: document.querySelector('input[name="password"]').value,
    };

    fetch('/api/login', { // 注意修改为你的后端登录API的URL
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(formData)
    })
    .then(response => response.json())
    .then(data => {
        console.log('Success:', data);
        alert('登录成功');
        // 登录成功后的操作，例如跳转到主页或用户仪表板
        window.location.href = 'index.html';
    })
    .catch((error) => {
        console.error('Error:', error);
        alert('登录失败：用户名或密码错误');
    });
});
