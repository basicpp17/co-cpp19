Project {
    name: "Tests"
    references: [
        "array19.lib/array19/array19.tests.qbs",
        "coro19.lib/coro19/coro19.tests.qbs",
        "enum19.lib/enum19/enum19.tests.qbs",
        "flags19.lib/flags19/flags19.tests.qbs",
        "meta19.lib/meta19/meta19.tests.qbs",
        "lookup19.lib/lookup19/lookup19.tests.qbs",
        "optional19.lib/optional19/optional19.tests.qbs",
        "partial19.lib/partial19/partial19.tests.qbs",
        "serialize19.lib/serialize19/serialize19.tests.qbs",
        "signal19.lib/signal19/signal19.tests.qbs",
        "string19.lib/string19/string19.tests.qbs",
        "strong19.lib/strong19/strong19.tests.qbs",
        "tuple19.lib/tuple19/tuple19.tests.qbs",
        "variant19.lib/variant19/variant19.tests.qbs",
    ]

    AutotestRunner {}
}
