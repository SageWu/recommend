let recommend = require("../build/Release/recommend.node");
// let recommend = require("../build/Debug/obj.target/recommend.node");

let scores = [
    [ 82, 1, 81, 0, 0, 0, 0 ],
    [ 81, 1, 0, 81, 1, 0, 0 ],
    [ 0, 0, 81, 0, 0, 81, 1 ]
];

let result = recommend.getCFRecommendations(scores, 0, {  }, (e) => {
    console.log(e);
});
console.log(result);

// let query = "基于海量词库的单词拼写检查、推荐到底是咋做的";
// let docs = [
//     "基于海量词库的单词拼写检查、推荐到底是咋做的"
// ];

// let result = recommend.tfidf(docs, query, (e) => {
//     console.log(e);
// });
// console.log(result);